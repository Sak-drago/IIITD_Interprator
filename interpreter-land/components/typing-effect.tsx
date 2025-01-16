'use client'

import { useEffect, useState } from "react"

const words = ["simple", "efficient", "fun", "IIIT-like"]
const TYPING_SPEED = 100
const DELETING_SPEED = 50
const PAUSE_TIME = 2000

export default function TypingEffect() {
  const [text, setText] = useState("")
  const [wordIndex, setWordIndex] = useState(0)
  const [isDeleting, setIsDeleting] = useState(false)
  const [isPaused, setIsPaused] = useState(false)

  useEffect(() => {
    const timeout = setTimeout(() => {
      if (isPaused) {
        setIsPaused(false)
        setIsDeleting(true)
        return
      }

      if (isDeleting) {
        if (text === "") {
          setIsDeleting(false)
          setWordIndex((prev) => (prev + 1) % words.length)
        } else {
          setText(text.slice(0, -1))
        }
      } else {
        const currentWord = words[wordIndex]
        if (text === currentWord) {
          setIsPaused(true)
        } else {
          setText(currentWord.slice(0, text.length + 1))
        }
      }
    }, isPaused ? PAUSE_TIME : isDeleting ? DELETING_SPEED : TYPING_SPEED)

    return () => clearTimeout(timeout)
  }, [text, wordIndex, isDeleting, isPaused])

  return (
    <span className="inline-block min-w-[120px] text-blue-900">
      {text}
      <span className="animate-blink">|</span>
    </span>
  )
}

