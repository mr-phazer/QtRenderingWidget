#pragma once

#include <memory>

namespace ttree
{

	template <typename DATA_TYPE>
	class TTreeNode
	{
		using SPTreeNode = std::shared_ptr<TTreeNode>;
		
	public:
		DATA_TYPE& Data() { return m_data; }

		DxBaseNode* GetParent() { return m_wpoParent.get(); };

		size_t GetChildCount() const {return m_children.size();	}

		void AddChild(SPTreeNode spoChild)
		{
			if (!spoChild) {
				return;
			}

			spoChild->SetParent(this);
			m_children.push_back(spoChild);
		}

	private:
		std::vector<SPTreeNode> m_children;
		std::weak_ptr<TTreeNode> m_wpoParent = nullptr;

		DATA_TYPE m_data;
	};

}