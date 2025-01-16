'use client'

import { useState } from "react"
import { Card } from "@/components/ui/card"

export default function CodePreview() {
  const [code] = useState(`# Example IIIT Interpreter code
<span class="text-purple-500">Plag</span> x = real

<span class="text-blue-500">Fn</span> <span class="text-green-500">dacCase</span>(x) {
    <span class="text-red-500">if</span> (<span class="text-blue-500">Fn</span>(didCase, didDac)) {
        return cooked
    }
}

# Execute the function
result = <span class="text-green-500">dacCase</span>(x)
<span class="text-yellow-500">print</span>(f"Result: {result}")`)

  const [output] = useState("Result: cooked")

  return (
    <Card className="bg-white shadow-xl overflow-hidden">
      <div className="grid md:grid-cols-2">
        {/* Code Editor */}
        <div className="bg-gray-900 p-4">
          <pre className="text-white font-mono text-sm">
            <code dangerouslySetInnerHTML={{ __html: code }}></code>
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

