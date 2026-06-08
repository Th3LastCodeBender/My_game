#triangle vertex shader
#version 410 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
};

#triangle fragment shader
#version 410 core	

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0);
};

// static std::string *parseShader(const std::string &filepath, std::string type)
// {
// 	std::ifstream		stream(filepath);
// 	std::string			strFuncts[2];
// 	std::string			line;
// 	std::stringstream 	stringStream;
// 	int					attribute;

// 	attribute = -1;
// 	while (getline(stream, line))
// 	{
// 		if (attribute != -1)
// 		{
// 			stringStream << line;
// 			if (line.empty() || line[0] == '\0' || line.find("};") != std::string::npos)
// 			{
// 				strFuncts[attribute] = stringStream.str();
// 				stringStream.clear();
// 				attribute = -1;		
// 			}
// 			continue;
// 		}
// 		if (line.find('#' + type) != std::string::npos)
// 		{
// 			if (line.find('#' + type + "fragment shader") != std::string::npos)
// 				attribute = VERTEX;
// 			else if (line.find('#' + type + "vertex shader") != std::string::npos)
// 				attribute = FRAGMENT;
// 		}
// 	}
// 	if (strFuncts[0].empty() || strFuncts[0][0] == '\0' || strFuncts[1].empty() || strFuncts[1][0] == '\0')
// 		std::cerr	<< "[parseShader]: Something went wrong while parsing the " 
// 					<< type 
// 					<< " shader function!" 
// 					<< std::endl;
// 	return (strFuncts);
// }