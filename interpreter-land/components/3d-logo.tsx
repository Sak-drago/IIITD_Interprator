'use client'

import { Canvas } from '@react-three/fiber'
import { Environment, Float } from '@react-three/drei'
import { useRef } from 'react'
import { Mesh } from 'three'

function Bar({ position, scale = [0.3, 2, 0.3] }: { position: [number, number, number], scale?: [number, number, number] }) {
  return (
    <mesh position={position}>
      <boxGeometry args={scale} />
      <meshStandardMaterial color="#666666" metalness={0.8} roughness={0.2} />
    </mesh>
  )
}

function Globe({ position }: { position: [number, number, number] }) {
  const sphereRef = useRef<Mesh>(null)
  const ringRef = useRef<Mesh>(null)

  return (
    <group position={position}>
      <mesh ref={sphereRef}>
        <sphereGeometry args={[0.3, 32, 32]} />
        <meshStandardMaterial color="#666666" metalness={0.8} roughness={0.2} />
      </mesh>
      <mesh ref={ringRef} rotation={[Math.PI / 2, 0, 0]}>
        <torusGeometry args={[0.4, 0.03, 16, 32]} />
        <meshStandardMaterial color="#666666" metalness={0.8} roughness={0.2} />
      </mesh>
    </group>
  )
}

export function Logo3D() {
  return (
    <div className="h-96 w-192">
      <Canvas camera={{ position: [0, 0, 5], fov: 45 }}>
        <Environment preset="city" />
        <ambientLight intensity={0.5} />
        <pointLight position={[10, 10, 10]} />
        
        <Float
          speed={3} 
          rotationIntensity={0.5} 
          floatIntensity={0.5}
          floatingRange={[-0.1, 0.1]}
        >
          <group position={[-0.6, 0, 0]} scale={0.6}>
            <Bar position={[-0.4, 0, 0]} />
            <Bar position={[0.4, 0, 0]} />
            <Bar position={[1.2, 0, 0]} />
            <Bar position={[2.0, 0, 0]} scale={[0.3, 2, 0.2]} />
            <Globe position={[3.0, 0, 0]} />
          </group>
        </Float>
      </Canvas>
    </div>
  )
}

