target_sources(RheelEngine PRIVATE
        RheelEngine.h
        RheelEngine/Color.cpp RheelEngine/Color.h
        RheelEngine/Component.cpp RheelEngine/Component.h
        RheelEngine/EngineResources.cpp RheelEngine/EngineResources.h
        RheelEngine/Game.cpp RheelEngine/Game.h
        RheelEngine/Material.cpp RheelEngine/Material.h
        RheelEngine/PhysicsShape.cpp RheelEngine/PhysicsShape.h
        RheelEngine/Scene.cpp RheelEngine/Scene.h
        RheelEngine/Task.h
        RheelEngine/ThreadPool.cpp RheelEngine/ThreadPool.h
        RheelEngine/Transform.cpp RheelEngine/Transform.h
        RheelEngine/_common.h
        RheelEngine/Animator/Animator.cpp RheelEngine/Animator/Animator.h
        RheelEngine/Animator/Clip.cpp RheelEngine/Animator/Clip.h
        RheelEngine/Animator/ConstantInterpolator.h
        RheelEngine/Animator/CosineInterpolator.h
        RheelEngine/Animator/CubicInterpolator.h
        RheelEngine/Animator/Interpolator.h
        RheelEngine/Animator/LinearInterpolator.h
        RheelEngine/Animator/Sequence.h
        RheelEngine/Animator/SplineInterpolator.h
        RheelEngine/Animator/Transition.cpp RheelEngine/Animator/Transition.h
        RheelEngine/Animator/TransitionInterpolator.h
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
        RheelEngine/Assets/Generators/StaticModelGeneratorCone.cpp RheelEngine/Assets/Generators/StaticModelGeneratorCone.h
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
        RheelEngine/Audio/OpenAL/Buffer.cpp RheelEngine/Audio/OpenAL/Buffer.h
        RheelEngine/Audio/OpenAL/Listener.cpp RheelEngine/Audio/OpenAL/Listener.h
        RheelEngine/Audio/OpenAL/Source.cpp RheelEngine/Audio/OpenAL/Source.h
        RheelEngine/Components/AnimatorComponent.cpp RheelEngine/Components/AnimatorComponent.h
        RheelEngine/Components/Camera.cpp RheelEngine/Components/Camera.h
        RheelEngine/Components/CollisionComponent.h
        RheelEngine/Components/DebugLines.cpp RheelEngine/Components/DebugLines.h
        RheelEngine/Components/DirectionalLight.cpp RheelEngine/Components/DirectionalLight.h
        RheelEngine/Components/EulerController.cpp RheelEngine/Components/EulerController.h
        RheelEngine/Components/InputComponent.cpp RheelEngine/Components/InputComponent.h
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
        RheelEngine/Registry/ComponentStorage.cpp RheelEngine/Registry/ComponentStorage.h
        RheelEngine/Registry/EntityId.h
        RheelEngine/Registry/EntityStorage.h
        RheelEngine/Registry/Registry.cpp RheelEngine/Registry/Registry.h
        RheelEngine/Renderer/CustomShaderModelRenderer.cpp RheelEngine/Renderer/CustomShaderModelRenderer.h
        RheelEngine/Renderer/ForwardSceneRenderer.cpp RheelEngine/Renderer/ForwardSceneRenderer.h
        RheelEngine/Renderer/GameRenderer.cpp RheelEngine/Renderer/GameRenderer.h
        RheelEngine/Renderer/ImageTexture.cpp RheelEngine/Renderer/ImageTexture.h
        RheelEngine/Renderer/ModelRenderer.cpp RheelEngine/Renderer/ModelRenderer.h
        RheelEngine/Renderer/PostProcessingEffect.cpp RheelEngine/Renderer/PostProcessingEffect.h
        RheelEngine/Renderer/PostProcessingStack.cpp RheelEngine/Renderer/PostProcessingStack.h
        RheelEngine/Renderer/SceneRenderManager.cpp RheelEngine/Renderer/SceneRenderManager.h
        RheelEngine/Renderer/SceneRenderer.cpp RheelEngine/Renderer/SceneRenderer.h
        RheelEngine/Renderer/ShadowMap.cpp RheelEngine/Renderer/ShadowMap.h
        RheelEngine/Renderer/ShadowMapDirectional.cpp RheelEngine/Renderer/ShadowMapDirectional.h
        RheelEngine/Renderer/SkyboxRenderer.cpp RheelEngine/Renderer/SkyboxRenderer.h
        RheelEngine/Renderer/VoxelRenderer.cpp RheelEngine/Renderer/VoxelRenderer.h
        RheelEngine/Renderer/Display/DisplayConfiguration.cpp RheelEngine/Renderer/Display/DisplayConfiguration.h
        RheelEngine/Renderer/Display/DummyWindow.cpp RheelEngine/Renderer/Display/DummyWindow.h
        RheelEngine/Renderer/Display/MainWindow.cpp RheelEngine/Renderer/Display/MainWindow.h
        RheelEngine/Renderer/Display/Window.cpp RheelEngine/Renderer/Display/Window.h
        RheelEngine/Renderer/OpenGL/AbstractTexture.cpp RheelEngine/Renderer/OpenGL/AbstractTexture.h
        RheelEngine/Renderer/OpenGL/Buffer.cpp RheelEngine/Renderer/OpenGL/Buffer.h
        RheelEngine/Renderer/OpenGL/Capabilities.cpp RheelEngine/Renderer/OpenGL/Capabilities.h
        RheelEngine/Renderer/OpenGL/Context.cpp RheelEngine/Renderer/OpenGL/Context.h
        RheelEngine/Renderer/OpenGL/ContextBindings.cpp RheelEngine/Renderer/OpenGL/ContextBindings.h
        RheelEngine/Renderer/OpenGL/ContextEnables.cpp RheelEngine/Renderer/OpenGL/ContextEnables.h
        RheelEngine/Renderer/OpenGL/ContextFunctions.cpp RheelEngine/Renderer/OpenGL/ContextFunctions.h
        RheelEngine/Renderer/OpenGL/Debug.cpp RheelEngine/Renderer/OpenGL/Debug.h
        RheelEngine/Renderer/OpenGL/DrawIndirectBuffer.h
        RheelEngine/Renderer/OpenGL/Enums.h
        RheelEngine/Renderer/OpenGL/Framebuffer.cpp RheelEngine/Renderer/OpenGL/Framebuffer.h
        RheelEngine/Renderer/OpenGL/Handle.h
        RheelEngine/Renderer/OpenGL/Object.h
        RheelEngine/Renderer/OpenGL/Program.cpp RheelEngine/Renderer/OpenGL/Program.h
        RheelEngine/Renderer/OpenGL/Renderbuffer.cpp RheelEngine/Renderer/OpenGL/Renderbuffer.h
        RheelEngine/Renderer/OpenGL/Shader.cpp RheelEngine/Renderer/OpenGL/Shader.h
        RheelEngine/Renderer/OpenGL/Texture.cpp RheelEngine/Renderer/OpenGL/Texture.h
        RheelEngine/Renderer/OpenGL/Texture2D.cpp RheelEngine/Renderer/OpenGL/Texture2D.h
        RheelEngine/Renderer/OpenGL/Texture2DArray.cpp RheelEngine/Renderer/OpenGL/Texture2DArray.h
        RheelEngine/Renderer/OpenGL/Texture2DMultisample.cpp RheelEngine/Renderer/OpenGL/Texture2DMultisample.h
        RheelEngine/Renderer/OpenGL/Uniform.cpp RheelEngine/Renderer/OpenGL/Uniform.h
        RheelEngine/Renderer/OpenGL/VertexArray.cpp RheelEngine/Renderer/OpenGL/VertexArray.h
        RheelEngine/Renderer/PostProcessing/Bloom.cpp RheelEngine/Renderer/PostProcessing/Bloom.h
        RheelEngine/Renderer/Text/Encoding.cpp RheelEngine/Renderer/Text/Encoding.h
        RheelEngine/Renderer/Text/Font.cpp RheelEngine/Renderer/Text/Font.h
        RheelEngine/Renderer/Text/FontRenderer.cpp RheelEngine/Renderer/Text/FontRenderer.h
        RheelEngine/Renderer/Text/Glyph.cpp RheelEngine/Renderer/Text/Glyph.h
        RheelEngine/Renderer/Text/PreparedText.cpp RheelEngine/Renderer/Text/PreparedText.h
        RheelEngine/Renderer/Text/TextAlign.h
        RheelEngine/Renderer/Text/TextRenderer.cpp RheelEngine/Renderer/Text/TextRenderer.h
        RheelEngine/UI/Constraint.cpp RheelEngine/UI/Constraint.h
        RheelEngine/UI/Container.cpp RheelEngine/UI/Container.h
        RheelEngine/UI/Element.cpp RheelEngine/UI/Element.h
        RheelEngine/UI/Input.h
        RheelEngine/UI/InputCallback.h
        RheelEngine/UI/UI.cpp RheelEngine/UI/UI.h
        RheelEngine/UI/Elements/ButtonElement.cpp RheelEngine/UI/Elements/ButtonElement.h
        RheelEngine/UI/Elements/ColorElement.cpp RheelEngine/UI/Elements/ColorElement.h
        RheelEngine/UI/Elements/CrosshairElement.cpp RheelEngine/UI/Elements/CrosshairElement.h
        RheelEngine/UI/Elements/EmptyElement.cpp RheelEngine/UI/Elements/EmptyElement.h
        RheelEngine/UI/Elements/SceneElement.cpp RheelEngine/UI/Elements/SceneElement.h
        RheelEngine/UI/Elements/TextElement.cpp RheelEngine/UI/Elements/TextElement.h
        RheelEngine/UI/Elements/VignetteElement.cpp RheelEngine/UI/Elements/VignetteElement.h
        RheelEngine/Util/Cache.h
        RheelEngine/Util/Hashes.h
        RheelEngine/Util/Log.cpp RheelEngine/Util/Log.h
        RheelEngine/Util/Math.h
        RheelEngine/Util/MsTimer.h
        RheelEngine/Util/glm_debug.cpp RheelEngine/Util/glm_debug.h
        RheelEngine/Util/pseudo_static_pointer.h)
