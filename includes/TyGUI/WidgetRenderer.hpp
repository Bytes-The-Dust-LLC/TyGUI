#pragma once

//manages the ImGUI instance and renders widgets

#include <Wireframe/Renderpass.hpp>


#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_sdl2.h>

namespace TyGUI
{
	//defines a widget renderer create info
	struct WidgetRenderer_CreateInfo
	{
		bool isDynamicRender = false; //requires 1.3 and default dynamic rendering for Wireframe and TyGUI
	};

	//defines a widget renderer
	struct WidgetRenderer
	{
		VkDescriptorPool imguiPool;

		//inits
		inline bool Init(Wireframe::Device::GPU* GPU, Wireframe::Window::DesktopWindow* window, VkRenderPass& renderpass,
			const WidgetRenderer_CreateInfo info)
		{
			//1: create descriptor pool for IMGUI
			// the size of the pool is very oversize, but it's copied from imgui demo itself.
			VkDescriptorPoolSize pool_sizes[] =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
			};

			VkDescriptorPoolCreateInfo pool_info = {};
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = 1000;
			pool_info.poolSizeCount = std::size(pool_sizes);
			pool_info.pPoolSizes = pool_sizes;

			VK_CHECK(vkCreateDescriptorPool(GPU->device, &pool_info, nullptr, &imguiPool));

			// 2: initialize imgui library

			//this initializes the core structures of imgui
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			static std::string iniFile = "Widgets";
			io.IniFilename = iniFile.c_str();
			//io.WantSaveIniSettings = true;

			//this initializes imgui for SDL
			ImGui_ImplSDL2_InitForVulkan(window->_window);

			//this initializes imgui for Vulkan
			ImGui_ImplVulkan_InitInfo init_info = {};
			init_info.Instance = GPU->instance;
			init_info.PhysicalDevice = GPU->chosenGPU;
			init_info.Device = GPU->device;
			init_info.Queue = GPU->graphicsQueue;
			init_info.DescriptorPool = imguiPool;
			init_info.MinImageCount = 3;
			init_info.ImageCount = 3;
			init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
			init_info.RenderPass = renderpass;

			ImGui_ImplVulkan_Init(&init_info);

			//execute a gpu command to upload imgui font textures
			//immediate_submit([&](VkCommandBuffer cmd) {
			//	ImGui_ImplVulkan_CreateFontsTexture(cmd);
			//	});

			//clear font textures from cpu data
			//ImGui_ImplVulkan_DestroyFontUploadObjects();

			return true;
		}

		//shutsdown
		inline void Shutdown(Wireframe::Device::GPU* GPU)
		{
			ImGui_ImplSDL2_Shutdown();
			ImGui_ImplVulkan_Shutdown();
			vkDestroyDescriptorPool(GPU->device, imguiPool, nullptr);
		}

		//passes input data to the widgets
		inline void SendInputData(const SDL_Event* e)
		{
			ImGui_ImplSDL2_ProcessEvent(e);
		}

		//starts a frame
		inline void StartFrame()
		{
			//imgui new frame
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();
		}

		//ends a frame
	
		//renders a frame into a command buffer
		inline void Render(VkCommandBuffer& cmd)
		{
			//renders ImGUI data
			ImGui::Render();
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
		}
	};
}