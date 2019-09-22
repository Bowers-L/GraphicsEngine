#include "buffers/Buffers.h"

#include "Shader.h"
#include "Renderer.h"

namespace primitive {
	class Primitive {
	protected:
		//render data


		virtual void setupBuffers() {};
	public:
		virtual void draw(Shader& shader) {};
	};
}