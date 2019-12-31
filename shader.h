/*
 * shader.h
 *
 *  Created on: Dec 31, 2019
 *      Author: doug
 */

#ifndef SHADER_H_
#define SHADER_H_


GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)


class Shader
{
	std::string mFSsource;
	std::string mVSsource;
	std::string mGSsource;
	GLuint vID, fID, gID, pID;
	std::string load(std::string path);
public:
	Shader();
	~Shader();
	GLuint getpID();
	GLuint build();
	void loadFS(std::string path);
	void loadVS(std::string path);
	void loadGS(std::string path);
	void checkCompileErrors(GLuint shader, std::string type);
	void use();
	void setInt(const std::string &name, int value) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
};



#endif /* SHADER_H_ */
