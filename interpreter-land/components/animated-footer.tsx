'use client'

import { useEffect, useState } from 'react'
import Link from 'next/link'

export function AnimatedFooter() {
  const [isVisible, setIsVisible] = useState(false)

  useEffect(() => {
    const handleScroll = () => {
      const scrolled = window.scrollY > 100
      setIsVisible(scrolled)
    }

    window.addEventListener('scroll', handleScroll)
    return () => window.removeEventListener('scroll', handleScroll)
  }, [])

  return (
    <footer className="border-t bg-white">
      <div className="container mx-auto px-4 py-6">
        <div className="grid grid-cols-1 md:grid-cols-4 gap-8">
          <div>
            <h3 className="font-bold text-lg mb-4">IIIT Interpreter</h3>
            <p className="text-gray-600">
              Making code interpretation simple, efficient, fun, and IIIT-like
            </p>
          </div>
          <div>
            <h4 className="font-bold mb-4">Product</h4>
            <ul className="space-y-2">
              <li><Link href="/coming-soon" className="text-gray-600 hover:text-gray-900">Features</Link></li>
              <li><Link href="/coming-soon" className="text-gray-600 hover:text-gray-900">Documentation</Link></li>
            </ul>
          </div>
          <div>
            <h4 className="font-bold mb-4">Resources</h4>
            <ul className="space-y-2">
              <li><Link href="https://github.com" className="text-gray-600 hover:text-gray-900">GitHub</Link></li>
              <li><Link href="/coming-soon" className="text-gray-600 hover:text-gray-900">Blog</Link></li>
            </ul>
          </div>
          <div>
            <h4 className="font-bold mb-4">Legal</h4>
            <ul className="space-y-2">
              <li><Link href="/coming-soon" className="text-gray-600 hover:text-gray-900">Privacy</Link></li>
              <li><Link href="/coming-soon" className="text-gray-600 hover:text-gray-900">Terms</Link></li>
            </ul>
          </div>
        </div>
        <div className="mt-8 pt-8 border-t text-center">
          <p className="text-gray-600">&copy; {new Date().getFullYear()} IIIT Interpreter. All rights reserved.</p>
          <p className="text-sm text-gray-500 mt-2">
            Made with ❤️ by Sakshat Sachdeva and Asher
          </p>
        </div>
      </div>
    </footer>
  )
}

