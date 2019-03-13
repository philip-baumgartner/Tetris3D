#version 400

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;
uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;

const int MAX_LIGHTS=14;
struct Light{
	int Type;
	vec3 Color;
	vec3 Position;
	vec3 Direction;
	vec3 Attenuation;
	vec3 SpotRadius;
	int ShadowIndex;
};

uniform Lights {
	int LightCount;
	Light lights[MAX_LIGHTS];
};

float sat( in float a){
    return clamp(a, 0.0, 1.0);
}

void main(){
    vec4 DiffTex = texture( DiffuseTexture, Texcoord);
	vec4 NormalTex = texture(NormalTexture, Texcoord);
    if(DiffTex.a <0.3f) discard;
	vec3 N = normalize(Normal);
	vec3 E = normalize(EyePos-Position);
	vec3 L;
	vec3 H;
	vec3 DiffuseComponent;
	vec3 SpecularComponent;

	// ##### Aufgabe 3 #####

	mat3 transMat = mat3(Tangent, -Bitangent, Normal);
	vec3 nNormalMap = vec3(NormalTex.r*2-1, NormalTex.g*2-1, NormalTex.b*2-1);
	nNormalMap = transMat * nNormalMap;
	nNormalMap = normalize(nNormalMap);	// Womit verrechnen?

	// ##### Aufgabe 1, 2 #####

	vec4 Color = vec4(0,0,0,0);
	for(int i=0; i<LightCount;i++){

		// Pointlights 
		if(lights[i].Type == 0){
			float dist = length(lights[i].Position - Position); 
			float intensity = 1.0 / (lights[i].Attenuation.x + lights[i].Attenuation.y * dist + lights[i].Attenuation.z *dist * dist);
			 
			L = normalize(lights[i].Position-Position);
			H = normalize(E + L); // E+L muss normalisiert werden. Bei einer Halbierung hätte der Vektor nicht zwingend die Läge 1.

			DiffuseComponent = lights[i].Color * intensity * DiffuseColor * sat(dot(N,L));
			SpecularComponent = lights[i].Color * intensity * SpecularColor * pow( sat(dot(H,N)), SpecularExp);

			Color += vec4((DiffuseComponent )*DiffTex.rgb + SpecularComponent ,DiffTex.a);
		}

		// Directionallights
		if(lights[i].Type == 1){
			L = normalize(-lights[i].Direction);
			H = normalize(E + L);

			DiffuseComponent = lights[i].Color * DiffuseColor * sat(dot(N,L));
			SpecularComponent = lights[i].Color * SpecularColor * pow( sat(dot(H,N)), SpecularExp);

			Color += vec4((DiffuseComponent )*DiffTex.rgb + SpecularComponent ,DiffTex.a);
		}

		//Spotlights
		if(lights[i].Type == 2){
			L = normalize(lights[i].Position-Position);
			H = normalize(E + L);

			float sigma = acos(dot(normalize(-lights[i].Direction), L));
			float dist = length(L);
			float intensity = 1.0 / (lights[i].Attenuation.x + lights[i].Attenuation.y * dist + lights[i].Attenuation.z *dist * dist);
			intensity *= (1 - sat((sigma - lights[i].SpotRadius.x) / (lights[i].SpotRadius.y - lights[i].SpotRadius.x)));

			DiffuseComponent = lights[i].Color * intensity * DiffuseColor * sat(dot(N,L));
			SpecularComponent = lights[i].Color * intensity * SpecularColor * pow( sat(dot(H,N)), SpecularExp);

			Color += vec4((DiffuseComponent )*DiffTex.rgb + SpecularComponent ,DiffTex.a);
		}
	}

	Color += vec4(AmbientColor *DiffTex.rgb, DiffTex.a);
	
	FragColor = Color;
}
