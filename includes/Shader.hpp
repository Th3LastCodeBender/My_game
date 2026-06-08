#ifndef SHADER_HPP
#define 	SHADER_HPP

#include "header.hpp"

class Shader
{
	private:

		void checkErrors(unsigned int shader, const std::string &type) const;

	public:

		unsigned int id;

		Shader(const char *vertPath, const char *fragPath);
		void	use();
		void	setMat4(const std::string &name, const glm::mat4 &mat) const;
		void	setVec3(const std::string &name, const glm::vec3 &vec) const;

};

#endif
