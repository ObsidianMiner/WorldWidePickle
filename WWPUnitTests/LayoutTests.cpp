#include "pch.h"
#include "CppUnitTest.h"

#include <imgui/imgui.h>
#include "Layout/Layout.h"
#include "Website.h"
#include "Node.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WWPUnitTests
{
    TEST_CLASS(LayoutTests)
    {
    public:

        static void InitImGui()
        {
            ImGui::CreateContext();
        }

        static void CleanupImGui()
        {
            ImGui::DestroyContext();
        }

        TEST_CLASS_INITIALIZE(Setup)
        {
            InitImGui();
        }

        TEST_CLASS_CLEANUP(TearDown)
        {
            CleanupImGui();
        }

        // ------------------------------------------------------------
        // Helper to create a test website safely
        // ------------------------------------------------------------

        Website* CreateTestWebsite()
        {
            Tab* fakeTab = nullptr;
            Website* website = new Website("test", fakeTab);
            website->rootNode = std::make_shared<Node>(nullptr, "div", 0);
            website->loaded = true;
            return website;
        }

        // ------------------------------------------------------------
        // 1. Block children stack vertically
        // ------------------------------------------------------------

        TEST_METHOD(BlockChildrenStackVertically)
        {
            std::unique_ptr<Website> website(CreateTestWebsite());
            Node* root = website->rootNode.load().get();

            root->children.emplace_back(root, "div", 1);
            root->children.emplace_back(root, "div", 2);

            website->layout.LayoutChildren(*root, 500.0f, *website);

            Assert::AreEqual(0.0f, root->children[0].layout.localPos.y);
            Assert::IsTrue(
                root->children[1].layout.localPos.y >=
                root->children[0].layout.contentSize.y
            );
        }

        // ------------------------------------------------------------
        // 2. Inline elements stay on same line
        // ------------------------------------------------------------

        TEST_METHOD(InlineChildrenStayOnSameLine)
        {
            std::unique_ptr<Website> website(CreateTestWebsite());
            Node* root = website->rootNode.load().get();

            root->children.emplace_back(root, "#text", 1);
            root->children.emplace_back(root, "#text", 2);

            root->children[0].attributes["text"] = "Hello";
            root->children[1].attributes["text"] = "World";

            root->children[0].style["display"] = std::string("inline");
            root->children[1].style["display"] = std::string("inline");

            website->layout.LayoutChildren(*root, 500.0f, *website);

            Assert::AreEqual(
                root->children[0].layout.localPos.y,
                root->children[1].layout.localPos.y
            );
        }

        // ------------------------------------------------------------
        // 3. Margin style is applied correctly
        // ------------------------------------------------------------

        TEST_METHOD(MarginStyleSetsMargins)
        {
            std::unique_ptr<Website> website(CreateTestWebsite());
            Node* root = website->rootNode.load().get();

            root->children.emplace_back(root, "div", 1);
            root->children[0].style["margin"] = 10.0f;

            website->layout.LayoutChildren(*root, 500.0f, *website);

            Assert::AreEqual(10.0f, root->children[0].layout.topLeftMargin.x);
            Assert::AreEqual(10.0f, root->children[0].layout.bottomRightMargin.x);
            Assert::AreEqual(10.0f, root->children[0].layout.topLeftMargin.y);
        }

        // ------------------------------------------------------------
        // 4. Div takes full parent width
        // ------------------------------------------------------------

        TEST_METHOD(DivTakesParentWidth)
        {
            std::unique_ptr<Website> website(CreateTestWebsite());
            Node* root = website->rootNode.load().get();

            root->children.emplace_back(root, "div", 1);

            website->layout.LayoutChildren(*root, 800.0f, *website);

            Assert::AreEqual(
                800.0f,
                root->children[0].layout.contentSize.x
            );
        }

        // ------------------------------------------------------------
        // 5. Image uses Website image dimensions
        // ------------------------------------------------------------

        TEST_METHOD(ImageUsesWebsiteDimensions)
        {
            std::unique_ptr<Website> website(CreateTestWebsite());
            Node* root = website->rootNode.load().get();

            root->children.emplace_back(root, "img", 1);
            Node& imgNode = root->children[0];

            imgNode.attributes["src"] = "logo";

            PImage img{};
            img.width = 200;
            img.height = 100;

            website->images["logo"] = img;

            website->layout.LayoutChildren(*root, 500.0f, *website);

            Assert::AreEqual(200.0f, imgNode.layout.contentSize.x);
            Assert::AreEqual(100.0f, imgNode.layout.contentSize.y);
        }

        // ------------------------------------------------------------
        // 6. Global position accumulates parent offset
        // ------------------------------------------------------------

        TEST_METHOD(GetGlobalPositionAccumulates)
        {
            Node parent(nullptr, "div", 0);
            Node child(&parent, "div", 1);

            parent.layout.localPos = ImVec2(50, 100);
            child.layout.localPos = ImVec2(20, 30);

            ImVec2 global = Layout::GetGlobalPos(child);

            Assert::AreEqual(70.0f, global.x);
            Assert::AreEqual(130.0f, global.y);
        }
    };
}