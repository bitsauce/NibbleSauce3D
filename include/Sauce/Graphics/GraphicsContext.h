#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include <Sauce/Common.h>
#include <Sauce/Graphics/Shader.h>
#include <Sauce/Graphics/BlendState.h>
#include <Sauce/Graphics/TextureRegion.h>

BEGIN_SAUCE_NAMESPACE

class Vertex;
class RenderTarget2D;
class VertexBuffer;
class IndexBuffer;

/**
 * \brief Handles primitive rendering to the screen.
 */
class SAUCE_API GraphicsContext
{
	friend class Game;
	friend class Window;
public:

	// State
	struct State
	{
		State() :
			width(0),
			height(0),
			texture(nullptr),
			shader(nullptr),
			blendState(BlendState::PRESET_ALPHA_BLEND),
			renderTarget(nullptr)
		{
			transformationMatrixStack.push(Matrix4());
		}

		uint width;
		uint height;
		shared_ptr<Texture2D> texture;
		shared_ptr<Shader> shader;
		BlendState blendState;
		RenderTarget2D *renderTarget;
		stack<Matrix4> transformationMatrixStack;
		Matrix4 projectionMatrix;
	};

	// TODO: For every set*, add a push*/pop* which uses the state stack
	void pushState();
	void popState();

	/**
	 * Primitive types.
	 * For rendering different kinds of primitives.
	 */
	enum PrimitiveType
	{
		PRIMITIVE_POINTS = GL_POINTS,					///< Points. 1 vertex per primitive.
		PRIMITIVE_LINES = GL_LINES,						///< Lines. 2 vertex per primitive.
		PRIMITIVE_LINE_STRIP = GL_LINE_STRIP,			///< Line strip. 2 vertex for the first primitive, 1 for the next.
		PRIMITIVE_LINE_LOOP = GL_LINE_LOOP,				///< Line loop. n lines
		PRIMITIVE_TRIANGLES = GL_TRIANGLES,				///< Triangles. 3 vertex per primitive.
		PRIMITIVE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,	///< Triangle strip. 3 vertex for the first primitive, 1 vertex for the next.
		PRIMITIVE_TRIANGLE_FAN = GL_TRIANGLE_FAN		///< Triangle fan. 3 vertex for the first primitive, 1 vertex for the next.
	};

	/**
	 * Clearing buffer mask.
	 * Used for selecting how to clear the backbuffer.
	 */
	enum BufferMask
	{
		COLOR_BUFFER = GL_COLOR_BUFFER_BIT,		///< %Color buffer
		DEPTH_BUFFER = GL_DEPTH_BUFFER_BIT,		///< Depth buffer
		STENCIL_BUFFER = GL_STENCIL_BUFFER_BIT	///< Stencil buffer
	};

	/**
	 * Sets a render target. This means that everything drawn after this function call
	 * will be rendered to the given \p renderTarget instead of the screen.
	 * TODO: Implement a render target stack.
	 * \param renderTarget The target buffer to render to.
	 */
	void pushRenderTarget(RenderTarget2D *renderTarget);
	void popRenderTarget();

	/**
	 * Get current render target.
	 */
	RenderTarget2D *getRenderTarget() const
	{
		return m_stateStack.top().renderTarget;
	}
	
	/**
	 * Pushes a matrix onto the transformation matrix stack.
	 * The matrix is pushed as follows \code stack.push_back(stack.top() * mat) \endcode
	 * \param mat Matrix to push on the stack.
	 */
	void pushMatrix(const Matrix4 &mat);

	/**
	 * Pops the top matrix from the transformation matrix stack.
	 */
	bool popMatrix();

	/**
	* Gets the top transformation matrix
	*/
	Matrix4 topMatrix() const;
	
	/**
	 *	Clear matrix stack
	 */
	void clearMatrixStack();

	/**
	 * Set texture. Every primitive rendered after this will have \p texture
	 * applied to it.
	 * \param texture Texture to apply to the primitives.
	 */
	void setTexture(shared_ptr<Texture2D> texture);

	/**
	 * Gets the current texture.
	 */
	shared_ptr<Texture2D> getTexture() const;

	/**
	 * Set shader. Every vertex and fragment rendered after this will
	 * have the effect of \p shader applied to them.
	 * \param shader Shader to render the primitves with.
	 */
	void setShader(shared_ptr<Shader> shader);

	/**
	 * Returns the current shader.
	 */
	shared_ptr<Shader> getShader() const;

	/**
	 * Set blend state. Every pixel rendered after this will use a 
	 * formula defined by \p blendState to blend new pixels with the back buffer.
	 * \param blendState The blend state to render with.
	 */
	void setBlendState(const BlendState &blendState);

	/**
	 * Returns the current blend state.
	 */
	BlendState getBlendState();

	/**
	 * Resizes the viewport (the area of the screen rendered to).
	 * \param w Width of the viewport in pixels
	 * \param h Height of the viewport in pixels
	 */
	void setSize(const uint w, const uint h);

	/**
	 * Set projection matrix
	 */
	void setProjectionMatrix(const Matrix4 matrix);

	/**
	 * Returns the width of the viewport.
	 */
	uint getWidth() const
	{
		return m_stateStack.top().width;
	}

	/**
	 * Returns the height of the viewport.
	 */
	uint getHeight() const
	{
		return m_stateStack.top().height;
	}

	/**
	* Returns the size of the viewport.
	*/
	Vector2I getSize() const
	{
		return Vector2I(m_stateStack.top().width, m_stateStack.top().height);
	}

	/**
	 * Renders a rectangle.
	 * \param rect Rectangle to render.
	 * \param color %Color of the rectangle.
	 * \param textureRegion Texture region of the rectangle.
	 */
	void drawRectangle(const Rect<float> &rect, const Color &color = Color::White, const TextureRegion &textureRegion = TextureRegion());

	/**
	 * Renders a rectangle.
	 * \param pos Top-left corner of the rectangle.
	 * \param size Size of the rectangle.
	 * \param color %Color of the rectangle.
	 * \param textureRegion Texture region of the rectangle.
	 */
	void drawRectangle(const Vector2F &pos, const Vector2F &size, const Color &color = Color::White, const TextureRegion &textureRegion = TextureRegion());

	/**
	 * Renders a rectangle.
	 * \param x Left x position of the rectangle.
	 * \param y Top y position of the rectangle.
	 * \param width Width of the rectangle.
	 * \param height Height of the rectangle.
	 * \param color %Color of the rectangle.
	 * \param textureRegion Texture region of the rectangle.
	 */
	void drawRectangle(const float x, const float y, const float width, const float height, const Color &color = Color::White, const TextureRegion &textureRegion = TextureRegion());

	/**
	* Renders a rectangle outline.
	* \param rect Rectangle to render.
	* \param color %Color of the rectangle.
	* \param textureRegion Texture region of the rectangle.
	*/
	void drawRectangleOutline(const Rect<float> &rect, const Color &color = Color::White, const TextureRegion &textureRegion = TextureRegion());

	/**
	* Renders a rectangle outline.
	* \param pos Top-left corner of the rectangle.
	* \param size Size of the rectangle.
	* \param color %Color of the rectangle.
	* \param textureRegion Texture region of the rectangle.
	*/
	void drawRectangleOutline(const Vector2F &pos, const Vector2F &size, const Color &color = Color::White, const TextureRegion &textureRegion = TextureRegion());

	/**
	* Renders a rectangle outline.
	* \param x Left x position of the rectangle.
	* \param y Top y position of the rectangle.
	* \param width Width of the rectangle.
	* \param height Height of the rectangle.
	* \param color %Color of the rectangle.
	* \param textureRegion Texture region of the rectangle.
	*/
	void drawRectangleOutline(const float x, const float y, const float width, const float height, const Color &color = Color::White, const TextureRegion &textureRegion = TextureRegion());

	/**
	 * Renders a circle.
	 * \param pos Center of the circle.
	 * \param radius Radius of the circle.
	 * \param segments Number of triangle segments to divide the circle into.
	 * \param color %Color of the circle.
	 */
	void drawCircleGradient(const Vector2F &pos, const float radius, const uint segments, const Color &center = Color::White, const Color &outer = Color::White);
	void drawCircle(const Vector2F &pos, const float radius, const uint segments, const Color &color = Color::White);

	/**
	 * Renders a circle.
	 * \param x Center x position of the circle.
	 * \param y Center y position of the circle.
	 * \param radius Radius of the circle.
	 * \param segments Number of triangle segments to divide the circle into.
	 * \param color %Color of the circle.
	 */
	void drawCircleGradient(const float x, const float y, const float radius, const uint segments, const Color &center = Color::White, const Color &outer = Color::White);
	void drawCircle(const float x, const float y, const float radius, const uint segments, const Color &color = Color::White);

	/**
	 * Draws an arrow from a starting point to an ending point.
	 * \param x0 Starting x-coordinate
	 * \param y0 Starting y-coordinate
	 * \param x1 Ending x-coordinate
	 * \param y1 Ending y-coordinate
	 * \param color %Color of the arrow
	 */
	void drawArrow(const float x0, const float y0, const float x1, const float y1, const Color &color = Color::White);
	void drawArrow(const Vector2F p0, const Vector2F p1, const Color &color = Color::White) { drawArrow(p0.x, p0.y, p1.x, p1.y, color); }

	void *getSDLHandle() const
	{
		return m_context;
	}

	Vertex *getVertices(const uint vertexCount);

protected:
	GraphicsContext();
	virtual ~GraphicsContext();

	void *m_context;
	Window *m_window;

	stack<State> m_stateStack;
	State *m_currentState;

	vector<Vertex> m_vertices; // Vertices for when needed

	static shared_ptr<Shader> s_defaultShader;
	static shared_ptr<Texture2D> s_defaultTexture;

public:
	/**
	 *  Pure virtual backend dependent functions
	 */
	 
	/**
	  * Graphics capabilites.
	  * For enabling and disabling certain rendering options.
	  */
	enum Capability
	{
		BLEND,					///< Back buffer blending
		DEPTH_TEST,				///< Depth testing
		FACE_CULLING,			///< Back face culling
		LINE_SMOOTH,			///< Smooth lines
		POLYGON_SMOOTH,			///< Smooth polygons
		MULTISAMPLE,			///< Multisample?
		TEXTURE_1D,				///< 1D textures
		TEXTURE_2D,				///< 2D textures
		TEXTURE_3D,				///< 3D textures
		VSYNC,
		WIREFRAME
	};

	/**
	 * Enables the capability \p cap.
	 * \param cap Capability to enable.
	 */
	virtual void enable(const Capability cap) = 0;

	/**
	 * Disables the capability \p cap.
	 * \param cap Capability to disable.
	 */
	virtual void disable(const Capability cap) = 0;

	/**
	 * Enable scissor testing
	 */
	virtual void enableScissor(const int x, const int y, const int w, const int h) = 0;
	
	/**
	 * Disable scissor testing
	 */
	virtual void disableScissor() = 0;

	/**
	 * Returns true if capability \p cap is enabled
	 */
	virtual bool isEnabled(const Capability cap) = 0;

	/**
	 * Set rendering point size
	 */
	virtual void setPointSize(const float pointSize) = 0;
	
	/**
	 * Set rendering line width
	 */
	virtual void setLineWidth(const float lineWidth) = 0;
	
	/**
	 * Set the size of the viewport
	 */
	virtual void setViewportSize(const uint w, const uint h) = 0;

	/**
	 * Clears the back buffer using \p mask.
	 * \param mask Decides what channels in the back buffer to clear.
	 * \param fillColor Decides what value to clear to.
	 */
	virtual void clear(const uint mask, const Color &fillColor = Color(0, 0, 0, 0)) = 0;

	/**
	 * Saves a screen shot of the back buffer to \p path as a PNG file.
	 * \param path Screen shot destination path
	 */
	virtual void saveScreenshot(string path) = 0;
	
	/**
	 * Create matricies
	 */
	virtual Matrix4 createOrtographicMatrix(const float left, const float right, const float top, const float bottom, const float n = -1.0f, const float f = 1.0f) const = 0;
	virtual Matrix4 createPerspectiveMatrix(const float fov, const float aspectRatio, const float zNear, const float zFar) const = 0;
	virtual Matrix4 createLookAtMatrix(const Vector3F &position, const Vector3F &fwd) const = 0;
	
	/**
	 * Renders an indexed primitive to the screen.
	 * \param type Types of primitives to render.
	 * \param vertices Array of vertices to render.
	 * \param vertexCount Number of vertices to render.
	 * \param indices Array of indices.
	 * \param indexCount Number of indices.
	 */
	virtual void drawIndexedPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount, const uint *indices, const uint indexCount) = 0;

	/**
	 * Renders an indexed primitive to the screen using vertex and index buffers.
	 * \param type Types of primitives to render.
	 * \param vbo Vertex buffer object.
	 * \param ibo Index buffer object.
	 */
	virtual void drawIndexedPrimitives(const PrimitiveType type, const VertexBuffer *vbo, const IndexBuffer *ibo) = 0;

	/**
	 * Renders primitives to the screen.
	 * \param type Types of primitives to render.
	 * \param vertices Array of vertices to render.
	 * \param vertexCount Number of vertices to render.
	 */
	virtual void drawPrimitives(const PrimitiveType type, const Vertex *vertices, const uint vertexCount) = 0;

	/**
	 * Renders primitives to the screen.
	 * \param type Types of primitives to render.
	 * \param vbo Vertex buffer object.
	 */
	virtual void drawPrimitives(const PrimitiveType type, const VertexBuffer *vbo) = 0;

	virtual Texture2D *createTexture(const Pixmap &pixmap) = 0;
	Texture2D *createTexture(const uint width, const uint height, const void *data = 0, const PixelFormat &format = PixelFormat());
	Texture2D *createTexture(const PixelFormat &format = PixelFormat());
	Texture2D *createTexture(const Texture2D &other);

	virtual Shader *createShader(const string &vertexSource, const string &fragmentSource, const string &geometrySource) = 0;
	virtual RenderTarget2D *createRenderTarget(const uint width, const uint height, const uint targetCount = 1, const PixelFormat &fmt = PixelFormat()) = 0;

protected:
	virtual Window *createWindow(const string &title, const int x, const int y, const int w, const int h, const Uint32 flags) = 0;
};

END_SAUCE_NAMESPACE

#endif // GRAPHICS_CONTEXT_H