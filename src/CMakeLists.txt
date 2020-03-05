target_sources(RheelEngine PRIVATE
		RheelEngine.h 
		RheelEngine/Color.cpp RheelEngine/Color.h 
		RheelEngine/Component.cpp RheelEngine/Component.h 
		RheelEngine/Engine.cpp RheelEngine/Engine.h 
		RheelEngine/EngineResources.cpp RheelEngine/EngineResources.h 
		RheelEngine/Entity.cpp RheelEngine/Entity.h 
		RheelEngine/Game.h 
		RheelEngine/Material.cpp RheelEngine/Material.h 
		RheelEngine/PhysicsShape.cpp RheelEngine/PhysicsShape.h 
		RheelEngine/RigidTransform.cpp RheelEngine/RigidTransform.h 
		RheelEngine/Scene.cpp RheelEngine/Scene.h 
		RheelEngine/Transform.cpp RheelEngine/Transform.h 
		RheelEngine/_common.h 
		RheelEngine/Assets/Asset.h 
		RheelEngine/Assets/AssetLoader.h 
		RheelEngine/Assets/Image.cpp RheelEngine/Assets/Image.h 
		RheelEngine/Assets/Model.cpp RheelEngine/Assets/Model.h 
		RheelEngine/Assets/Shader.cpp RheelEngine/Assets/Shader.h 
		RheelEngine/Assets/Sound.cpp RheelEngine/Assets/Sound.h 
		RheelEngine/Assets/VoxelImage.cpp RheelEngine/Assets/VoxelImage.h 
		RheelEngine/Assets/Generators/StaticModelGenerator.cpp RheelEngine/Assets/Generators/StaticModelGenerator.h 
		RheelEngine/Assets/Generators/StaticModelGeneratorBox.cpp RheelEngine/Assets/Generators/StaticModelGeneratorBox.h 
		RheelEngine/Assets/Generators/StaticModelGeneratorCapsule.cpp RheelEngine/Assets/Generators/StaticModelGeneratorCapsule.h 
		RheelEngine/Assets/Generators/StaticModelGeneratorSphere.cpp RheelEngine/Assets/Generators/StaticModelGeneratorSphere.h 
		RheelEngine/Assets/Loaders/ColladaLoader.cpp RheelEngine/Assets/Loaders/ColladaLoader.h 
		RheelEngine/Assets/Loaders/GlslLoader.cpp RheelEngine/Assets/Loaders/GlslLoader.h 
		RheelEngine/Assets/Loaders/Loader.h 
		RheelEngine/Assets/Loaders/PngLoader.cpp RheelEngine/Assets/Loaders/PngLoader.h 
		RheelEngine/Assets/Loaders/VoxelLoader.cpp RheelEngine/Assets/Loaders/VoxelLoader.h 
		RheelEngine/Assets/Loaders/WaveLoader.cpp RheelEngine/Assets/Loaders/WaveLoader.h 
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
		RheelEngine/Components/Skybox.cpp RheelEngine/Components/Skybox.h 
		RheelEngine/Components/SpotLight.cpp RheelEngine/Components/SpotLight.h 
		RheelEngine/Components/VoxelRenderComponent.cpp RheelEngine/Components/VoxelRenderComponent.h 
		RheelEngine/Renderer/CustomShaderModelRenderer.cpp RheelEngine/Renderer/CustomShaderModelRenderer.h 
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
		RheelEngine/Renderer/SkyboxRenderer.cpp RheelEngine/Renderer/SkyboxRenderer.h 
		RheelEngine/Renderer/VoxelRenderer.cpp RheelEngine/Renderer/VoxelRenderer.h 
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
