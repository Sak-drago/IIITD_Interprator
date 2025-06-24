"use client"

import { useState } from "react"
import Editor from "@monaco-editor/react"
import { Button } from "@/components/ui/button"
import Navigation from "./navigation"
import { AnimatedFooter } from "./animated-footer"

export function Interpreter() {
  const [code, setCode] = useState("")
  const [output, setOutput] = useState("")
  const [isLoading, setIsLoading] = useState(false)

  const handleRunCode = async () => {
    setIsLoading(true)
    setOutput("")
    try {
      const response = await fetch("/api/interpret", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ code }),
      })
      const result = await response.json()
      if (response.ok) {
        setOutput(result.output)
      } else {
        setOutput(`Error: ${result.error}`)
      }
    } catch (error) {
      setOutput("Error: Unable to connect to the server.")
    }
    setIsLoading(false)
  }

  return (
    <div className="flex min-h-screen flex-col bg-gray-950 text-white">
      <Navigation />
      <main className="flex-grow flex flex-col">
        <div className="flex-1 grid grid-cols-1 md:grid-cols-2 gap-4 p-4">
          <div className="flex flex-col rounded-lg border border-gray-700">
            <div className="flex-1">
              <Editor
                height="100%"
                defaultLanguage="plaintext"
                value={code}
                onChange={(value: string | undefined) => setCode(value || "")}
                theme="vs-dark"
              />
            </div>
            <div className="flex items-center justify-between border-t border-gray-700 bg-gray-900 p-2">
              <Button
                onClick={handleRunCode}
                disabled={isLoading}
                variant="default"
              >
                {isLoading ? "Running..." : "Run Code"}
              </Button>
            </div>
          </div>
          <div className="flex flex-col rounded-lg border border-gray-700">
            <div className="bg-gray-900 p-2">
              <h2 className="text-lg font-semibold">Output</h2>
            </div>
            <div className="flex-1 bg-black p-4">
              <pre className="whitespace-pre-wrap text-sm">{output}</pre>
            </div>
          </div>
        </div>
      </main>
      <AnimatedFooter />
    </div>
  )
} 