#include "Rasterizer.h"
#include "../Maths/Assert.h"

void TV::Renderer::RenderContext::Validate() const
{
	if (!IsValid())
	{
		return;
	}
	if (DepthBuffer != nullptr)
	{
		check(DepthBuffer->GetSize() == Canvas->GetSize());
	}
}
