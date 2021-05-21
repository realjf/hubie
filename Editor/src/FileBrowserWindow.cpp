#include "FileBrowserWindow.h"
#include "Editor.h"
#include <Hubie/Core/StringUtilities.h>
#include <Hubie/Core/OS/FileSystem.h>
#include <Hubie/ImGui/IconsMaterialDesignIcons.h>
#include <imgui/imgui.h>
#include <imgui/plugins/ImFileBrowser.h>

namespace Hubie
{
	FileBrowserWindow::FileBrowserWindow()
	{
		m_Name = "FileBrowserWindow";
		m_SimpleName = "FileBrowser";

		m_FileBrowser = new ImGui::FileBrowser(ImGuiFileBrowserFlags_CreateNewDir | ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_NoModal);
		m_FileBrowser->SetTitle("Test File Browser");
		//m_FileBrowser->SetFileFilters({ ".sh" , ".h" });
		m_FileBrowser->SetLabels(ICON_MDI_FOLDER, ICON_MDI_FILE, ICON_MDI_FOLDER_OPEN);
		m_FileBrowser->Refresh();
	}

	FileBrowserWindow::~FileBrowserWindow()
	{
		delete m_FileBrowser;
	}

	void FileBrowserWindow::OnImGui()
	{
		m_FileBrowser->Display();

		if(m_FileBrowser->HasSelected())
		{
			std::string tempFilePath = m_FileBrowser->GetSelected().string();

			std::string filePath = Hubie::StringUtilities::BackSlashesToSlashes(tempFilePath);

			m_Callback(filePath);

			m_FileBrowser->ClearSelected();
		}
	}

	void FileBrowserWindow::Open()
	{
		m_FileBrowser->Open();
	}
}
