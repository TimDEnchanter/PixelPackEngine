//FRAGMENT SHADER

#version 330

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
#define MAX_POINT_LIGHTS 25

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define MAX_DIR_LIGHTS 25

struct Material {
	vec3 ambient;
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;
uniform DirLight dirLights[MAX_DIR_LIGHTS];
uniform int numDirLights;
uniform Material material;

in vec3 viewPos;

in vec3 vertPos;
in vec3 vertNorm;
in vec2 vertUV;

out vec4 fragColor;

vec3 addDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//find light vector
	vec3 lightDir = normalize(-light.direction);

	//calculate diffuse intensity
	float diff = clamp( dot(normal, lightDir), 0.0, 1.0 );

	//calculate specular intensity
	float spec;
	if ( dot(normal, lightDir) < 0.0)
	{
		spec = 0.0;  //wrong side
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow( max( dot(viewDir, reflectDir), 0.0 ), material.shininess );
	}

	//combine
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, vertUV));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vertUV));
	vec3 specular = light.specular * spec * material.specular;

	return( ambient + diffuse + specular );
}

vec3 addPointLight(PointLight light, vec3 normal, vec3 vertPos, vec3 viewDir)
{
	//find light vector
	vec3 lightDir = normalize(light.position - vertPos);

	//calculate diffuse intensity
	float diff = clamp( dot(normal, lightDir), 0.0, 1.0 );

	//calculate specular intensity
	float spec;
	if ( dot(normal, lightDir) < 0.0)
	{
		spec = 0.0;  //wrong side
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow( max( dot(viewDir, reflectDir), 0.0 ), material.shininess );
	}

	//calculate attenuation intensity
	float lightDist = length( light.position - vertPos );
	float atten = 1.0 / ( light.constant + light.linear * lightDist + light.quadratic * (lightDist * lightDist) );

	//combine
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, vertUV));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vertUV));
	vec3 specular = light.specular * spec * material.specular;
	ambient *= atten;
	diffuse *= atten;
	specular *= atten;

	return( ambient + diffuse + specular );
}

void main()
{
	//normalize properties
	vec3 normalized = normalize(vertNorm);
	vec3 viewDir = normalize(viewPos - vertPos);

	//base value
	vec3 output = vec3(0.0);

	//add point lights
	for (int i=0; i<numPointLights; i++)
		output += addPointLight(pointLights[i], normalized, vertPos, viewDir);

	//add direction lights
	for (int i=0; i<numDirLights; i++)
		output += addDirLight(dirLights[i], normalized, viewDir);

	fragColor = vec4(output, 1.0);
	//fragColor = vec4(normalized, 1.0);
}
