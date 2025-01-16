import Image from 'next/image'
import { Card } from '@/components/ui/card'
import { Github } from 'lucide-react'
import { AnimatedSection } from './animated-section'
import { Logo3D } from "./3d-logo"
import Link from "next/link"

export function FeaturesSection() {
  return (
    <div className="w-full py-24 space-y-32">
      {/* Ready for Interpretation */}
      <AnimatedSection>
      <section className="container px-4 mx-auto">
        <h2 className="text-4xl font-bold text-center mb-6">Ready for Interpretation</h2>
        <p className="text-xl text-center text-gray-600 mb-12">
          With IIIT Interpreter, you can execute code, visualize data, and generate outputs in real-time.
        </p>
        <div className="grid md:grid-cols-2 gap-8 items-center">
          <Card className="p-6">
            <pre className="text-sm">
              <code>{`# Example IIIT Interpreter code
Plag x = real

Fn dacCase(x) {
    if (Fn(didCase, didDac)) {
        return cooked
    }
}

# Execute the function
result = dacCase(x)
print(f"Result: {result}")`}</code>
            </pre>
          </Card>
          <div className="bg-gray-100 p-6 rounded-lg">
            <h3 className="text-2xl font-bold mb-4">Contribute Now!</h3>
            <p className="text-gray-700 mb-4">
              Help shape the future of IIIT Interpreter by creating your own libraries and extensions.
            </p>
            <Link href="/coming-soon">
              <button className="bg-blue-600 text-white px-6 py-2 rounded-lg font-medium hover:bg-blue-700 transition-colors">
                Learn How to Contribute
              </button>
            </Link>
          </div>
        </div>
      </section>
      </AnimatedSection>

      {/* Writing Code Section */}
      <AnimatedSection>
      <section className="container px-4 mx-auto">
        <h2 className="text-4xl font-bold text-center mb-6">Writing Code has Never Been Easier</h2>
        <p className="text-xl text-center text-gray-600 mb-12">
          Say goodbye to complex development environments and setup procedures.
        </p>
        <div className="max-w-3xl mx-auto">
          <Image 
            src="https://hebbkx1anhila5yf.public.blob.vercel-storage.com/image-OfsrLonGPyr3rBlQpQWORwS3sHHwwI.png"
            alt="Code writing example"
            width={800}
            height={400}
            className="rounded-lg shadow-lg"
          />
        </div>
      </section>
      </AnimatedSection>

      {/* Open Source Section */}
      <AnimatedSection>
      <section className="container px-4 mx-auto text-center">
        <Logo3D />
        <h2 className="text-4xl font-bold mb-6">Open Source</h2>
        <div className="flex items-center justify-center gap-4 mb-6">
          <Image
            src="https://hebbkx1anhila5yf.public.blob.vercel-storage.com/IIIT_Lang_Logo-removebg-preview-pOUNhKw3Lp8LnnEuCq23s2vuF40GoQ.png"
            alt="IIIT Logo"
            width={100}
            height={50}
            className="opacity-80"
          />
          <span className="text-4xl">+</span>
          <Github className="w-12 h-12" />
        </div>
        <p className="text-xl text-gray-600 max-w-2xl mx-auto">
          IIIT Interpreter is Open Source! You can follow along with the development on GitHub.
        </p>
      </section>
      </AnimatedSection>

      {/* Coming Soon Section */}
      <AnimatedSection>
      <section className="container px-4 mx-auto text-center">
        <h2 className="text-4xl font-bold mb-6">There&apos;s more!</h2>
        <p className="text-xl text-gray-600 mb-8">
          There is still so much more we want to do. Coming soon:
        </p>
        <ul className="space-y-4 text-lg">
          <li>Advanced debugging tools</li>
          <li>Language Server Protocol</li>
          <li>Custom language extensions</li>
        </ul>
      </section>
      </AnimatedSection>

      {/* Final CTA */}
      <AnimatedSection>
      <section className="bg-gradient-to-br from-teal-400 to-blue-500 py-20">
        <div className="container px-4 mx-auto text-center text-white">
          <h2 className="text-4xl font-bold mb-6">Try IIIT Interpreter soon!</h2>
          <p className="text-xl mb-8">
            We&apos;re working hard to bring you the best code interpretation experience.
          </p>
          <div className="flex gap-4 justify-center">
            <Link href="/coming-soon">
              <button className="bg-white text-blue-600 px-6 py-3 rounded-lg font-medium hover:bg-white/90">
                Coming Soon
              </button>
            </Link>
            <Link href="https://github.com/Sak-drago/IIITD_Interprator/">
              <button className="border border-white text-white px-6 py-3 rounded-lg font-medium hover:bg-white/10">
                View on GitHub
              </button>
            </Link>
          </div>
        </div>
      </section>
      </AnimatedSection>
    </div>
  )
}

