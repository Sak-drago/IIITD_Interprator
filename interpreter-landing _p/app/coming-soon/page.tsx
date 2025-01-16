import { Button } from "@/components/ui/button"
import Link from "next/link"

export default function ComingSoon() {
  return (
    <div className="min-h-screen flex flex-col">
      <main className="flex-1 flex flex-col items-center justify-center px-4 bg-gradient-to-br from-teal-400 to-blue-500">
        <div className="max-w-4xl w-full text-center">
          <h1 className="text-5xl font-bold text-white mb-6">
            Coming Soon
          </h1>
          <p className="text-xl text-white/90 mb-12">
            We&apos;re working hard to bring you something amazing. Stay tuned!
          </p>
          <Link href="/">
            <Button size="lg" variant="default" className="bg-white text-blue-600 hover:bg-white/90">
              Back to Home
            </Button>
          </Link>
        </div>
      </main>
    </div>
  )
}

