import { NextResponse } from "next/server";

export async function POST(req: Request) {
  try {
    const { code } = await req.json();

    // Here you would call your actual interpreter service
    // For now, we'll just echo the code back
    const output = `Executing code:\n\n${code}`;

    return NextResponse.json({ output });
  } catch (error) {
    return NextResponse.json(
      { error: "An error occurred while interpreting the code." },
      { status: 500 }
    );
  }
}
