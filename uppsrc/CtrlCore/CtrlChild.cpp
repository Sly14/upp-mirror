#include "CtrlCore.h"

#define LLOG(x)  // LOG(x)

void  Ctrl::AddChild(Ctrl *q, Ctrl *p)
{
	ASSERT(q);
	LLOG("Add " << ::Name(q) << " to: " << Name());
	if(p == q) return;
	bool updaterect = true;
	if(q->parent) {
		ASSERT(!q->inframe);
		if(q->parent == this) {
			RemoveChild0(q);
			updaterect = false;
		}
		else
			q->parent->RemoveChild(q);
	}
	q->parent = this;
	if(p) {
		ASSERT(p->parent == this);
		q->prev = p;
		q->next = p->next;
		if(p == lastchild)
			lastchild = q;
		else
			p->next->prev = q;
		p->next = q;
	}
	else
		if(firstchild) {
			q->prev = NULL;
			q->next = firstchild;
			firstchild->prev = q;
			firstchild = q;
		}
		else {
			ASSERT(lastchild == NULL);
			firstchild = lastchild = q;
			q->prev = q->next = NULL;
		}
	q->CancelModeDeep();
	if(updaterect)
		q->UpdateRect();
	ChildAdded(q);
	q->ParentChange();
	if(GetTopCtrl()->IsOpen())
		q->StateH(OPEN);
}

void Ctrl::AddChild(Ctrl *child)
{
	AddChild(child, lastchild);
}

void Ctrl::AddChildBefore(Ctrl *child, Ctrl *insbefore)
{
	if(insbefore)
		AddChild(child, insbefore->prev);
	else
		AddChild(child);
}

void  Ctrl::RemoveChild0(Ctrl *q)
{
	ChildRemoved(q);
	q->DoRemove();
	q->parent = NULL;
	if(q == firstchild)
		firstchild = firstchild->next;
	if(q == lastchild)
		lastchild = lastchild->prev;
	if(q->prev)
		q->prev->next = q->next;
	if(q->next)
		q->next->prev = q->prev;
	q->next = q->prev = NULL;
}

void  Ctrl::RemoveChild(Ctrl *q)
{
	if(q->parent != this) return;
	q->RefreshFrame();
	RemoveChild0(q);
	q->ParentChange();
	if(GetTopCtrl()->IsOpen())
		q->StateH(CLOSE);
}

void  Ctrl::Remove()
{
	if(parent)
		parent->RemoveChild(this);
}

bool Ctrl::HasChild(Ctrl *q) const
{
	return q && q->IsChild() && q->parent == this;
}

bool Ctrl::HasChildDeep(Ctrl *q) const
{
	while(q && q->IsChild()) {
		if(q->parent == this) return true;
		q = q->parent;
	}
	return false;
}

static bool IterateFocusFw(Ctrl *ctrl, bool noframe, bool init, bool all)
{
	LLOG("IterateFocusFw(" << ::Name(ctrl) << ")");
	while(ctrl) {
		if(ctrl->IsOpen() && ctrl->IsVisible() && ctrl->IsEnabled()) {
			if(!(noframe && ctrl->InFrame())) {
				if(all) {
					ctrl->SetFocus();
					return true;
				}
				if((!init || ctrl->IsInitFocus()) && ctrl->SetWantFocus())
					return true;
			}
			if(IterateFocusFw(ctrl->GetFirstChild(), noframe, init, all))
				return true;
		}
		ctrl = ctrl->GetNext();
	}
	return false;
}

bool Ctrl::IterateFocusForward(Ctrl *ctrl, Ctrl *top, bool noframe, bool init, bool all)
{
	LLOG("IterateFocusForward(" << ::Name(ctrl) << ", top " << ::Name(top) << ", noframe " << noframe << ", init " << init << ")");
	if(!ctrl) return false;
	if(IterateFocusFw(ctrl->GetFirstChild(), noframe, init, all))
		return true;
	if(ctrl->GetNext() && IterateFocusFw(ctrl->GetNext(), noframe, init, all))
		return true;
	while(ctrl->GetParent() != top && (ctrl = ctrl->GetParent()) != NULL)
		if(IterateFocusFw(ctrl->GetNext(), noframe, init, all))
			return true;
	return false;
}

static bool IterateFocusBw(Ctrl *ctrl, bool noframe, bool all)
{
	while(ctrl) {
		if(ctrl->IsOpen() && ctrl->IsVisible() && ctrl->IsEnabled()) {
			if(IterateFocusBw(ctrl->GetLastChild(), noframe, all))
				return true;
			if(!(noframe && ctrl->InFrame())) {
				if(all) {
					ctrl->SetFocus();
					return true;
				}
				if(ctrl->SetWantFocus())
					return true;
			}
		}
		ctrl = ctrl->GetPrev();
	}
	return false;
}

bool Ctrl::IterateFocusBackward(Ctrl *ctrl, Ctrl *top, bool noframe, bool all)
{
	if(!ctrl || ctrl == top) return false;
	if(IterateFocusBw(ctrl->GetPrev(), noframe, all))
		return true;
	while(ctrl->GetParent() != top) {
		ctrl = ctrl->GetParent();
		if(ctrl->SetWantFocus())
			return true;
		if(IterateFocusBw(ctrl->GetPrev(), noframe, all))
			return true;
	}
	return false;
}

Ctrl *Ctrl::GetTopCtrl()
{
	Ctrl *q = this;
	while(q->parent)
		q = q->parent;
	return q;
}

const Ctrl *Ctrl::GetTopCtrl() const      { return const_cast<Ctrl *>(this)->GetTopCtrl(); }
const Ctrl *Ctrl::GetOwner() const        { return const_cast<Ctrl *>(this)->GetOwner(); }
Ctrl       *Ctrl::GetTopCtrlOwner()       { return GetTopCtrl()->GetOwner(); }
const Ctrl *Ctrl::GetTopCtrlOwner() const { return GetTopCtrl()->GetOwner(); }

TopWindow *Ctrl::GetTopWindow()
{
	Ctrl *q = GetTopCtrl();
	while(q) {
		TopWindow *w = dynamic_cast<TopWindow *>(q);
		if(w) return w;
		q = q->GetOwner();
	}
	return NULL;
}

const TopWindow *Ctrl::GetTopWindow() const
{
	return const_cast<Ctrl *>(this)->GetTopWindow();
}

TopWindow *Ctrl::GetMainWindow()
{
	Ctrl *q = GetTopCtrl();
	for(;;) {
		Ctrl *w = q->GetOwner();
		if(!w)
			return dynamic_cast<TopWindow *>(q);
		q = w;
	}
}

const TopWindow *Ctrl::GetMainWindow() const
{
	return const_cast<Ctrl *>(this)->GetMainWindow();
}
