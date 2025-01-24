#include "../include/threadPool.h"
#include "../include/asserts.h"
#include "../include/logger.h"

bool taskQueueInit(TaskQueue* QUEUE, int CAPACITY)
{
  FORGE_ASSERT_MESSAGE(QUEUE != NULL, "Cannot init a null task queue");
  FORGE_ASSERT_MESSAGE(CAPACITY > 0, "Task Queue must have a positive capacity");

  QUEUE->tasks      = malloc(sizeof(Task) * CAPACITY);
  QUEUE->front      =   0;
  QUEUE->rear       =  -1;
  QUEUE->count      =   0;
  QUEUE->capacity   = CAPACITY;
  
  if (QUEUE->tasks  == 0)
  {
    FORGE_LOG_ERROR("THREAD_POOL_TASK_QUEUE : Malloc failed to make a task queue, size : %d", CAPACITY * sizeof(Task));
    return false;
  }

  FORGE_LOG_DEBUG("THREAD_POOL_TASK_QUEUE : creating the readLock");
  pthread_mutex_init(&QUEUE->readLock, NULL);
  FORGE_LOG_DEBUG("THREAD_POOL_TASK_QUEUE : creating the availability cond_t");
  pthread_cond_init(&QUEUE->availability, NULL);

  FORGE_LOG_INFO("THREAD_POOL_TASK_QUEUE : initialized with %d capacity", CAPACITY);
  return true;
}

void taskQueuePush(TaskQueue* QUEUE, Task TASK)
{
  FORGE_LOG_TRACE("THREAD_POOL_TASK_QUEUE : Waiting to get the readLock");
  pthread_mutex_lock(&QUEUE->readLock);
  FORGE_LOG_DEBUG("= = = QUEUE : Entering Critical Section = = =");

  while (QUEUE->count == QUEUE->capacity)
  {
    FORGE_LOG_TRACE("THREAD_POOL_TASK_QUEUE : Waiting for task availability");
    FORGE_LOG_TRACE("THREAD_POOL_TASK_QUEUE : Count %d : capacity: %d", QUEUE->count, QUEUE->capacity);
    pthread_cond_wait(&QUEUE->availability, &QUEUE->readLock);
  }

  FORGE_LOG_INFO("THREAD_POOL_TASK_QUEUE : Adding task to queue");
  QUEUE->rear = (QUEUE->rear + 1) % QUEUE->capacity;
  QUEUE->count++;
  QUEUE->tasks[QUEUE->rear] = TASK;

  FORGE_LOG_DEBUG("THREAD_POOL_TASK_QUEUE : Signaling task availability");
  pthread_cond_signal(&QUEUE->availability);
  FORGE_LOG_DEBUG("= = = QUEUE : Exiting Critical Section = = =");
  pthread_mutex_unlock(&QUEUE->readLock);
}

void taskQueueDestroy(TaskQueue* QUEUE)
{
  FORGE_LOG_WARNING("THREAD_POOL_TASK_QUEUE : I got fired too, was I not a nice manager :(");
  free(QUEUE->tasks);
  pthread_mutex_destroy(&QUEUE->readLock);
  pthread_cond_destroy(&QUEUE->availability);
}

void* workerThread(void* ARG)
{
  ThreadPool*          pool   = (ThreadPool*)ARG;
  FORGE_ASSERT_MESSAGE(pool   != NULL, "THREAD_POOL_WORKER : ThreadPool is NULL somehow. Panic");
  
  while (true)
  {
    FORGE_LOG_TRACE("THREAD_POOL_WORKER (%d) : Waiting to get the readLock", pthread_self());
    pthread_mutex_lock(&pool->queue.readLock);
    FORGE_LOG_DEBUG("= = = THREAD_POOL_WORKER (%d) : Entering Critical Section = = =");

    while (pool->queue.count == 0 && !pool->stop)
    {
      FORGE_LOG_TRACE("THREAD_POOL_WORKER (%d) : Waiting for task availability", pthread_self());
      pthread_cond_wait(&pool->queue.availability, &pool->queue.readLock);
    }

    if (pool->stop)
    {
      FORGE_LOG_WARNING("THREAD_POOL_WORKER (%d) : I got fired :(", pthread_self());
      FORGE_LOG_DEBUG("= = = THREAD_POOL_WORKER (%d) : Exiting Critical Section = = =", pthread_self());
      pthread_mutex_unlock(&pool->queue.readLock);
      break;
    }

    Task task           = pool->queue.tasks[pool->queue.front];
    pool->queue.front   = (pool->queue.front + 1) % pool->queue.capacity;
    pool->queue.count--;
    FORGE_LOG_TRACE("THREAD_POOL_WORKER (%d) got assigned a task", pthread_self());

    pthread_cond_signal(&pool->queue.availability);
    pthread_mutex_unlock(&pool->queue.readLock);

    task.function(task.argument);
    FORGE_LOG_TRACE("THREAD_POOL_WORKER (%d) finished a task", pthread_self());
  }
  return NULL;
}

bool createThreadPool(ThreadPool* POOL, int THREAD_COUNT, int TASK_CAPACITY)
{
  FORGE_ASSERT_MESSAGE(POOL           != NULL, "Cannot initialize a NULL pool");
  FORGE_ASSERT_MESSAGE(THREAD_COUNT   > 0    , "Must have atleast one thread in the thread pool");
  FORGE_ASSERT_MESSAGE(TASK_CAPACITY  > 0    , "Must have atleast one slot for tasks");

  POOL->threadCount = THREAD_COUNT;
  POOL->stop        = false;
  POOL->threads     = malloc(THREAD_COUNT * sizeof(Thread));
  if (POOL->threads == 0)
  {
    FORGE_LOG_ERROR("THREAD_POOL : Failed on malloc for threads. Thread Count : %d", THREAD_COUNT);
    return false;
  }

  if (!taskQueueInit(&POOL->queue, TASK_CAPACITY))
  {
    FORGE_LOG_ERROR("THREAD_POOL : failed to create the task queue");
  };

  for (int i = 0; i < THREAD_COUNT; ++i)
  {
    FORGE_LOG_INFO("THREAD_POOL : Hiring worker %d of %d", i + 1, THREAD_COUNT);
    if (pthread_create(&POOL->threads[i], NULL, workerThread, POOL) != 0)
    {
      FORGE_LOG_ERROR("THREAD_POOL : Failed to create worker thread");
    };
  }

  return true;
}

void pushTask(ThreadPool* POOL, void (*FUNCTION)(void *), void* ARGUMENT) 
{
  FORGE_ASSERT_MESSAGE(FUNCTION != NULL, "A task cannot be null");

  Task task;
  task.function = FUNCTION;
  task.argument = ARGUMENT;

  taskQueuePush(&POOL->queue, task);
}

bool destroyThreadPool(ThreadPool *POOL)
{
  FORGE_LOG_WARNING("THREAD_POOL : destroying everything, firing everyone!");
  pthread_mutex_lock(&POOL->queue.readLock);
  POOL->stop = true;
  pthread_cond_broadcast(&POOL->queue.availability);
  pthread_mutex_unlock(&POOL->queue.readLock);
  for (int i = 0; i < POOL->threadCount; ++i)
  {
    FORGE_LOG_TRACE("THREAD_POOL : waiting for worker thread %d", i + 1);
    pthread_join(POOL->threads[i], NULL);
  }
  free(POOL->threads);
  taskQueueDestroy(&POOL->queue);

  return true;
}
