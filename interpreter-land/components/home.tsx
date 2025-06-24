import Link from "next/link"
import { Github } from 'lucide-react'
import { Button } from "@/components/ui/button"
import CodePreview from "./code-preview"
import Navigation from "./navigation"
import TypingEffect from "./typing-effect"
import { FeaturesSection } from "./features-section"
import { AnimatedFooter } from "./animated-footer"

export default function Home() {
  return (
    <div className="min-h-screen flex flex-col">
      {/* Announcement banner */}
      <div className="bg-black text-white text-center p-2 text-sm">
        <Link href="/coming-soon">
          Learn what&apos;s new in IIIT Interpreter 1.0
        </Link>
      </div>

      <Navigation />

      {/* Main content */}
      <main className="flex-1 flex flex-col">
        {/* Hero Section */}
        <div className="w-full bg-gradient-to-br from-teal-400 to-blue-500 px-4 pb-20">
          <div className="max-w-4xl w-full mx-auto text-center pt-20 pb-16">
            <h1 className="text-5xl font-bold text-white mb-6">
              Code interpretation made <TypingEffect />
            </h1>
            <p className="text-xl text-white/90 mb-12">
              Focus on your code and let IIIT Interpreter handle the execution and output formatting.
            </p>
            
            <div className="flex gap-4 justify-center">
              <Link href="/interpreter">
                <Button size="lg" variant="default" className="bg-white text-blue-600 hover:bg-white/90">
                  Open Interpreter
                </Button>
              </Link>
              <Link href="https://github.com/Sak-drago/IIITD_Interprator/">
                <Button size="lg" variant="outline" className="bg-transparent border-white text-white hover:bg-white/10">
                  <Github className="mr-2 h-5 w-5" />
                  View on GitHub
                </Button>
              </Link>
            </div>
          </div>

          {/* Code Preview Section */}
          <div className="w-full max-w-5xl mx-auto">
            <CodePreview />
          </div>
        </div>

        {/* Features Section */}
        <FeaturesSection />
      </main>

      {/* Footer */}
      <AnimatedFooter />
    </div>
  )
}

