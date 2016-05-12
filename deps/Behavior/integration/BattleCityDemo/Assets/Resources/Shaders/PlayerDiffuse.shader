Shader "BattleCityShader/PlayerDiffuse" {
	Properties {
		_MainTex ("Base (RGB)", 2D) = "white" {}
		_Color ("Main Color", Color) = (1,1,1,1)
		_AddColor("Add Color", Color) = (0,0,0,1)
	}
	 
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		
		CGPROGRAM
		#pragma surface surf Lambert noforwardadd

		sampler2D _MainTex;
		fixed4 _Color;
		float4 _AddColor;

		struct Input 
		{
			float2 uv_MainTex;
			float3 worldPos;
		};
		
		void surf (Input IN, inout SurfaceOutput o) 
		{			
			half4 c = tex2D (_MainTex, IN.uv_MainTex)*_Color;
			o.Albedo = c.rgb;
			o.Alpha = c.a;
			
			o.Emission = _AddColor;
		}
		ENDCG
	} 
	
	FallBack "Diffuse"
}
