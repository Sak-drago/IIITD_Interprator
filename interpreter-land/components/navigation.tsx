import Link from "next/link"
import { Button } from "@/components/ui/button"
import Image from "next/image"
import { GitHubSignIn } from "./github-signin"

export default function Navigation() {
  return (
    <nav className="border-b bg-white">
      <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
        <div className="flex justify-between h-16 items-center">
          <div className="flex items-center">
            <Link href="/" className="flex items-center gap-2">
             <Image
              src="https://hebbkx1anhila5yf.public.blob.vercel-storage.com/IIIT_Lang_Logo-removebg-preview-pOUNhKw3Lp8LnnEuCq23s2vuF40GoQ.png"
              alt="IIIT Logo"
              width={100}
              height={50}
              className="opacity-80"
            />
            </Link>
          </div>

          <div className="hidden md:flex items-center space-x-4">
            <Link href="/coming-soon" className="text-gray-600 hover:text-gray-900">
              Documentation
            </Link>
            <Link href="/coming-soon" className="text-gray-600 hover:text-gray-900">
              Examples
            </Link>
            <Link href="/coming-soon" className="text-gray-600 hover:text-gray-900">
              Community
            </Link>
            <GitHubSignIn />
          </div>
        </div>
      </div>
    </nav>
  )
}

