#pragma once
#include "Catalyst/Levels/Level.h"

using Catalyst::Levels::Level;

const char* const EDITOR_LEVEL_NAME = "editor";

class TestNetworkActor;

class EditorLevel final : public Level
{
public:
	EditorLevel();

protected:
	void OnLoaded() override;
	void OnUnloaded() override;

private:
	TestNetworkActor* m_testNetworkActor;

};