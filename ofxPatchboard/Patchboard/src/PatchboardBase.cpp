#include "PatchboardBase.h"

size_t PatchboardBase::getNumPatches()
{
	return patchcords.size();
}

std::string PatchboardBase::getLastErrMsg()
{
	return _lastErrMsg;
}

PatchboardBase& PatchboardBase::operator=(const PatchboardBase & p)
{
	inputType = p.inputType;
	outputType = p.outputType;
	patchcords = p.patchcords;

	return *this;
}
