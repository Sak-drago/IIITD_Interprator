'use client'

import { useState } from "react"
import { Card } from "@/components/ui/card"

export default function CodePreview() {
  const [code] = useState(`# Example IIIT Interpreter code
def factorial(n):
    if n == 0:
        return 1
    return n * factorial(n-1)

# Calculate factorial of 5
result = factorial(5)
print(f"Factorial of 5 is {result}")`)

  const [output] = useState("Factorial of 5 is 120")

  return (
    <Card className="bg-white shadow-xl overflow-hidden">
      <div className="grid md:grid-cols-2">
        {/* Code Editor */}
        <div className="bg-gray-900 p-4">
          <pre className="text-white font-mono text-sm">
            <code>{code}</code>
          </pre>
        </div>

        {/* Output */}
        <div className="p-4 bg-white border-l">
          <h3 className="text-sm font-semibold mb-2">Output:</h3>
          <pre className="font-mono text-sm text-gray-700">
            {output}
          </pre>
        </div>
      </div>
    </Card>
  )
}

