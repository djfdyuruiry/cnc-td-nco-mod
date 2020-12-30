#pragma once

#include "game_messages.h"

void ShowGameNotification(const char* message, float durationInSeconds);

void RefreshSidebar();

void RefreshGameMap();

void RevealEntireMapToPlayer();

void HideEntireMapFromPlayer();

bool ProcessGameUiMessage(GameUiMessage& uiMessage);
