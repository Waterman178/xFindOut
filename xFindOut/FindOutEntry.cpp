#include "FindOutEntry.h"
#include "pluginsdk/_plugins.h"

FindOutEntry::FindOutEntry(duint breakpointAddress) :
    breakpointAddress(breakpointAddress),
    dialog(breakpointAddress)
{

}

duint FindOutEntry::getBreakpointAddress()
{
    return breakpointAddress;
}

int FindOutEntry::wasHitBefore(duint cip)
{
    auto it = std::find_if(hits.begin(), hits.end(), [cip](const auto& hit) {
        return hit->hittedAtAddress == cip;
    });

    if (it == hits.end())
        return -1;

    return std::distance(hits.begin(), it);
}

void FindOutEntry::hit(int index)
{
    if (index < 0 || index >= hits.size())
        return;

    hits[index]->hits++;
    dialog.updateHits(index, hits[index]->hits);
}

void FindOutEntry::addNew(HitEntry& hitEntry)
{
    hits.push_back(std::make_unique<HitEntry>(std::move(hitEntry)));
    dialog.insertRow(hits.back()->instruction);
}

HWND FindOutEntry::getDialog()
{
    return dialog.getHWND();
}

void FindOutEntry::debugLog()
{
    for (const auto& hit : hits)
        _plugin_logprintf("Address %p, %s, hits: %d\n%s\n\n\n", hit->instructionAddress, hit->instruction, hit->hits, hit->info);
}

char* FindOutEntry::getInfoByIndex(int index)
{
    if (index < 0 || index >= hits.size())
        return nullptr;

    return hits[index]->info;
}

duint FindOutEntry::getInstructionAddressByIndex(int index)
{
    if (index < 0 || index >= hits.size())
        return 0;

    return hits[index]->instructionAddress;
}
