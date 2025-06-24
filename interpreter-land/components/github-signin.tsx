'use client'

import { useState } from 'react'
import { Button } from "@/components/ui/button"
import { Github } from 'lucide-react'

export function GitHubSignIn() {
  const [isLoading, setIsLoading] = useState(false)

  const handleSignIn = async () => {
    setIsLoading(true)
    // Here you would typically initiate the OAuth flow
    // For now, we'll just simulate a delay
    await new Promise(resolve => setTimeout(resolve, 2000))
    setIsLoading(false)
    alert('GitHub sign-in functionality coming soon!')
  }

  return (
    <Button 
      variant="outline" 
      onClick={handleSignIn}
      disabled={isLoading}
    >
      {isLoading ? (
        <span className="loading loading-spinner loading-sm"></span>
      ) : (
        <Github className="mr-2 h-4 w-4" />
      )}
      Sign in with GitHub
    </Button>
  )
}

