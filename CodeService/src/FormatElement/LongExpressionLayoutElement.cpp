#include "CodeService/FormatElement/LongExpressionLayoutElement.h"
#include "CodeService/FormatElement/KeepElement.h"

LongExpressionLayoutElement::LongExpressionLayoutElement(int continuationIndent, bool isAssignLeftExprList)
	: _hasContinuation(false),
	  _continuationIndent(continuationIndent)
{
}

FormatElementType LongExpressionLayoutElement::GetType()
{
	return FormatElementType::LongExpressionLayoutElement;
}

void LongExpressionLayoutElement::Serialize(SerializeContext& ctx, ChildIterator selfIt, FormatElement& parent)
{
	SerializeSubExpression(ctx, *this);
	if (_hasContinuation)
	{
		ctx.RecoverIndent();
	}
}

void LongExpressionLayoutElement::Diagnosis(DiagnosisContext& ctx, ChildIterator selfIt, FormatElement& parent)
{
	DiagnosisSubExpression(ctx, *this);
	if (_hasContinuation)
	{
		ctx.RecoverIndent();
	}
}

void LongExpressionLayoutElement::SerializeSubExpression(SerializeContext& ctx, FormatElement& parent)
{
	auto& children = parent.GetChildren();
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		auto child = *it;

		if (child->GetType() == FormatElementType::SubExpressionElement)
		{
			SerializeSubExpression(ctx, *child);
		}
		else
		{
			child->Serialize(ctx, it, parent);
		}
		if (child->GetType() == FormatElementType::KeepElement)
		{
			if (ctx.GetCharacterCount() == 0 && !_hasContinuation)
			{
				_hasContinuation = true;
				auto state = ctx.GetCurrentIndent();
				state.SpaceIndent = _continuationIndent;
				ctx.AddIndent(state);
			}
		}
	}
}

void LongExpressionLayoutElement::DiagnosisSubExpression(DiagnosisContext& ctx, FormatElement& parent)
{
	auto& children = parent.GetChildren();
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		auto child = *it;

		if (child->GetType() == FormatElementType::SubExpressionElement)
		{
			DiagnosisSubExpression(ctx, *child);
		}
		else
		{
			child->Diagnosis(ctx, it, parent);
		}
		if (child->GetType() == FormatElementType::KeepElement)
		{
			if (ctx.GetCharacterCount() == 0 && !_hasContinuation)
			{
				_hasContinuation = true;
				auto state = ctx.GetCurrentIndent();
				state.SpaceIndent = _continuationIndent;
				ctx.AddIndent(state);
			}
		}
	}
}
