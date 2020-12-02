#pragma once



HRESULT ReflectShader(ID3DBlob* VBlob, ID3D11InputLayout** pOutInputLayout, ID3D11ShaderReflection** pOutReflection);
HRESULT CompileVS(string Path, string Entry, ID3D11VertexShader** ppVS, ID3D11InputLayout** ppIL);
HRESULT CompilePS(string Path, string Entry, ID3D11PixelShader** ppPS);
HRESULT CompilePassFromFile(string Path, string Entry, Pass* pPass);