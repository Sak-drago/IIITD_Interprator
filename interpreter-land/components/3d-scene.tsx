'use client'

import { Canvas } from '@react-three/fiber'
import { Environment, Float, Text3D, OrbitControls } from '@react-three/drei'
import { useRef } from 'react'
import { Mesh } from 'three'

export function Scene3D() {
  const meshRef = useRef<Mesh>(null)

  return (
    <div className="h-[400px] w-full">
      <Canvas camera={{ position: [0, 0, 10], fov: 45 }}>
        <Environment preset="city" />
        <ambientLight intensity={0.5} />
        <pointLight position={[10, 10, 10]} />
        
        <Float
          speed={4} 
          rotationIntensity={1} 
          floatIntensity={2}
          floatingRange={[-0.1, 0.1]}
        >
          <Text3D
            font="/fonts/Geist_Bold.json"
            size={1.5}
            height={0.2}
            curveSegments={12}
            bevelEnabled
            bevelThickness={0.02}
            bevelSize={0.02}
            bevelOffset={0}
            bevelSegments={5}
            position={[-4, 0, 0]}
          >
            IIIT
            <meshStandardMaterial color="#3b82f6" metalness={0.8} roughness={0.2} />
          </Text3D>
        </Float>

        <Float
          speed={4} 
          rotationIntensity={1} 
          floatIntensity={2}
          floatingRange={[-0.1, 0.1]}
        >
          <mesh ref={meshRef} position={[2, 0, 0]}>
            <torusKnotGeometry args={[1, 0.3, 128, 16]} />
            <meshStandardMaterial color="#22c55e" metalness={0.8} roughness={0.2} />
          </mesh>
        </Float>

        <OrbitControls enableZoom={false} />
      </Canvas>
    </div>
  )
}

