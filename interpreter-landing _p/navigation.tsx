import Link from "next/link"
import { Button } from "@/components/ui/button"

export default function Navigation() {
  return (
    <nav className="border-b bg-white">
      <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
        <div className="flex justify-between h-16 items-center">
          <div className="flex items-center">
            <Link href="/" className="text-2xl font-bold text-blue-600">
              IIIT Interpreter
            </Link>
          </div>

          <div className="hidden md:flex items-center space-x-4">
            <Link href="/docs" className="text-gray-600 hover:text-gray-900">
              Documentation
            </Link>
            <Link href="/examples" className="text-gray-600 hover:text-gray-900">
              Examples
            </Link>
            <Link href="/community" className="text-gray-600 hover:text-gray-900">
              Community
            </Link>
            <Button variant="ghost">Sign in</Button>
            <Button>Sign up</Button>
          </div>
        </div>
      </div>
    </nav>
  )
}

