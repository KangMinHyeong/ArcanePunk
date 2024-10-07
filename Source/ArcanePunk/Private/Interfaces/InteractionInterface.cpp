// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/InteractionInterface.h"
#include "Character/ArcanePunkCharacter.h"

bool IInteractionInterface::BeginFocus()
{
    // if true, When Focusing Actor Direct Act;
    return false;
}

// void IInteractionInterface::EndFocus()
// {

// }

FInteractData IInteractionInterface::GetInteractData()
{
    return FInteractData();
}

void IInteractionInterface::BeginInteract()
{

}

void IInteractionInterface::EndInteract()
{

}

void IInteractionInterface::Interact(AArcanePunkCharacter* PlayerCharacter)
{

}
