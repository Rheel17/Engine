target_sources(RheelEngine PRIVATE
		RheelEngine.h 
		RheelEngine/ColladaParser.cpp RheelEngine/ColladaParser.h 
		RheelEngine/Color.cpp RheelEngine/Color.h 
		RheelEngine/Component.cpp RheelEngine/Component.h 
		RheelEngine/Engine.cpp RheelEngine/Engine.h 
		RheelEngine/EngineResources.cpp RheelEngine/EngineResources.h 
		RheelEngine/Entity.cpp RheelEngine/Entity.h 
		RheelEngine/Game.h 
		RheelEngine/Image.cpp RheelEngine/Image.h 
		RheelEngine/Image3D.cpp RheelEngine/Image3D.h 
		RheelEngine/Image3DResource.cpp RheelEngine/Image3DResource.h 
		RheelEngine/Image3DVOX.cpp 
		RheelEngine/ImagePNG.cpp 
		RheelEngine/ImageResource.cpp RheelEngine/ImageResource.h 
		RheelEngine/Material.cpp RheelEngine/Material.h 
		RheelEngine/Model.cpp RheelEngine/Model.h 
		RheelEngine/ModelBuiltInBox.cpp 
		RheelEngine/ModelBuiltInCapsule.cpp 
		RheelEngine/ModelBuiltInSphere.cpp 
		RheelEngine/ModelResource.cpp RheelEngine/ModelResource.h 
		RheelEngine/PhysicsShape.cpp RheelEngine/PhysicsShape.h 
		RheelEngine/Resource.h 
		RheelEngine/ResourceManager.cpp RheelEngine/ResourceManager.h 
		RheelEngine/RigidTransform.cpp RheelEngine/RigidTransform.h 
		RheelEngine/Scene.cpp RheelEngine/Scene.h 
		RheelEngine/Sound.cpp RheelEngine/Sound.h 
		RheelEngine/SoundResource.cpp RheelEngine/SoundResource.h 
		RheelEngine/SoundWAV.cpp 
		RheelEngine/Transform.cpp RheelEngine/Transform.h 
		RheelEngine/_common.h 
		RheelEngine/Audio/AudioClip.cpp RheelEngine/Audio/AudioClip.h 
		RheelEngine/Audio/AudioManager.cpp RheelEngine/Audio/AudioManager.h 
		RheelEngine/Audio/AudioSource.cpp RheelEngine/Audio/AudioSource.h 
		RheelEngine/Audio/OpenAL/ALBuffer.cpp RheelEngine/Audio/OpenAL/ALBuffer.h 
		RheelEngine/Audio/OpenAL/ALListener.cpp RheelEngine/Audio/OpenAL/ALListener.h 
		RheelEngine/Audio/OpenAL/ALSource.cpp RheelEngine/Audio/OpenAL/ALSource.h 
		RheelEngine/Components/Camera.cpp RheelEngine/Components/Camera.h 
		RheelEngine/Components/CollisionComponent.cpp RheelEngine/Components/CollisionComponent.h 
		RheelEngine/Components/DirectionalLight.cpp RheelEngine/Components/DirectionalLight.h 
		RheelEngine/Components/EulerController.cpp RheelEngine/Components/EulerController.h 
		RheelEngine/Components/Light.cpp RheelEngine/Components/Light.h 
		RheelEngine/Components/ListenerFollow.cpp RheelEngine/Components/ListenerFollow.h 
		RheelEngine/Components/ModelRenderComponent.cpp RheelEngine/Components/ModelRenderComponent.h 
		RheelEngine/Components/PerspectiveCamera.cpp RheelEngine/Components/PerspectiveCamera.h 
		RheelEngine/Components/PhysicsScene.cpp RheelEngine/Components/PhysicsScene.h 
		RheelEngine/Components/PointLight.cpp RheelEngine/Components/PointLight.h 
		RheelEngine/Components/Rigidbody.cpp RheelEngine/Components/Rigidbody.h 
		RheelEngine/Components/SpotLight.cpp RheelEngine/Components/SpotLight.h 
		RheelEngine/Renderer/DisplayConfiguration.cpp RheelEngine/Renderer/DisplayConfiguration.h 
		RheelEngine/Renderer/ForwardSceneRenderer.cpp RheelEngine/Renderer/ForwardSceneRenderer.h 
		RheelEngine/Renderer/ImageTexture.cpp RheelEngine/Renderer/ImageTexture.h 
		RheelEngine/Renderer/ModelRenderer.cpp RheelEngine/Renderer/ModelRenderer.h 
		RheelEngine/Renderer/PostProcessingEffect.cpp RheelEngine/Renderer/PostProcessingEffect.h 
		RheelEngine/Renderer/PostProcessingStack.cpp RheelEngine/Renderer/PostProcessingStack.h 
		RheelEngine/Renderer/SceneRenderer.cpp RheelEngine/Renderer/SceneRenderer.h 
		RheelEngine/Renderer/SceneRenderManager.cpp RheelEngine/Renderer/SceneRenderManager.h 
		RheelEngine/Renderer/ShadowMap.cpp RheelEngine/Renderer/ShadowMap.h 
		RheelEngine/Renderer/ShadowMapDirectional.cpp RheelEngine/Renderer/ShadowMapDirectional.h 
		RheelEngine/Renderer/Window.cpp RheelEngine/Renderer/Window.h 
		RheelEngine/Renderer/OpenGL/GL.cpp RheelEngine/Renderer/OpenGL/GL.h 
		RheelEngine/Renderer/OpenGL/GLBuffer.cpp RheelEngine/Renderer/OpenGL/GLBuffer.h 
		RheelEngine/Renderer/OpenGL/GLFramebuffer.cpp RheelEngine/Renderer/OpenGL/GLFramebuffer.h 
		RheelEngine/Renderer/OpenGL/GLRenderbuffer.cpp RheelEngine/Renderer/OpenGL/GLRenderbuffer.h 
		RheelEngine/Renderer/OpenGL/GLShaderProgram.cpp RheelEngine/Renderer/OpenGL/GLShaderProgram.h 
		RheelEngine/Renderer/OpenGL/GLTexture2D.cpp RheelEngine/Renderer/OpenGL/GLTexture2D.h 
		RheelEngine/Renderer/OpenGL/GLTexture2DMultisample.cpp RheelEngine/Renderer/OpenGL/GLTexture2DMultisample.h 
		RheelEngine/Renderer/OpenGL/GLVertexArray.cpp RheelEngine/Renderer/OpenGL/GLVertexArray.h 
		RheelEngine/Renderer/PostProcessing/Bloom.cpp RheelEngine/Renderer/PostProcessing/Bloom.h 
		RheelEngine/Renderer/Text/Character.cpp RheelEngine/Renderer/Text/Character.h 
		RheelEngine/Renderer/Text/Font.cpp RheelEngine/Renderer/Text/Font.h 
		RheelEngine/Renderer/Text/TextRenderer.cpp RheelEngine/Renderer/Text/TextRenderer.h 
		RheelEngine/UI/Constraint.cpp RheelEngine/UI/Constraint.h 
		RheelEngine/UI/Container.cpp RheelEngine/UI/Container.h 
		RheelEngine/UI/Element.cpp RheelEngine/UI/Element.h 
		RheelEngine/UI/Input.h 
		RheelEngine/UI/InputCallback.h 
		RheelEngine/UI/UI.cpp RheelEngine/UI/UI.h 
		RheelEngine/UI/Elements/ColorElement.cpp RheelEngine/UI/Elements/ColorElement.h 
		RheelEngine/UI/Elements/CrosshairElement.cpp RheelEngine/UI/Elements/CrosshairElement.h 
		RheelEngine/UI/Elements/EmptyElement.cpp RheelEngine/UI/Elements/EmptyElement.h 
		RheelEngine/UI/Elements/SceneElement.cpp RheelEngine/UI/Elements/SceneElement.h 
		RheelEngine/UI/Elements/TextElement.cpp RheelEngine/UI/Elements/TextElement.h 
		RheelEngine/Util/glm_debug.cpp RheelEngine/Util/glm_debug.h 
		RheelEngine/Util/Hashes.h 
		RheelEngine/Util/Log.cpp RheelEngine/Util/Log.h 
		RheelEngine/Util/Math.h 
		RheelEngine/Util/MsTimer.h 
)
