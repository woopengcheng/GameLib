Shader "BattleCityShader/BubbleDamage" 
{
	Properties 
	{
		_diffuseTex ("Base Texture", 2D) = "white" {}
		//_alphaThreshold ("Alpha threshold", Range(0.0, 1.0)) = 0.5
		_colorBias ("TextColor", Color) = (1.0, 1.0, 1.0, 1.0)
	}
	
	SubShader 
	{
		Tags {"Queue"="Transparent+2000" "RenderType"="Transparent"}
		Cull Off
		Blend SrcAlpha OneMinusSrcAlpha
		ZWrite Off
		Pass{
			CGPROGRAM
			#pragma vertex	vert
			#pragma fragment frag
			#include "UnityCG.cginc"
	
			sampler2D 	_diffuseTex;
			//float 	_alphaThreshold;
			float4 		_colorBias;
			
	
			struct VSOUPUT{
				float4 pos:SV_POSITION;
				float2 uv:TEXCOORD0;
			};
			
			VSOUPUT vert(appdata_full v)
			{
				VSOUPUT o;
				o.pos	=	mul(UNITY_MATRIX_MVP,v.vertex);
				o.uv	=	v.texcoord;//TRANSFORM_TEX(v.texcoord,_MainTex);
				return o;
			};
		
			half4 frag(VSOUPUT IN) :COLOR
			{
				half4 c 	= tex2D(_diffuseTex, IN.uv);
				//clip(c.a - 0.1f);
				
				return float4(c.xyz*_colorBias.xyz*2.0f,c.w*_colorBias.w);
			}
			ENDCG
		}
	} 
	Fallback "Diffuse"
}
