#pragma once

#include <QList>
#include <QRegularExpression>

#include <Qt/utils/util.h>
#include <Qt/objects/rom_data.h>

using MimicRomDataQList = QList<QList<RomData*>>;
using RomDataQList = QList<RomData>;

// See namespace ComparePtrs
using CompareFuncPtr = bool(*)(const RomData&, const RomData&);

class RomDataContainer
{

public:
    RomDataContainer();
    explicit RomDataContainer(const RomDataQList &data);
    ~RomDataContainer();

    void update_raw_data();

    void set_raw_data(const RomDataQList &data);
    const RomDataQList* get_raw_data() const;

    const MimicRomDataQList* get_mimic_data() const;

    const RomData& get_romdata(const int &page, const int &index) const;

    void sort_data(const CompareFuncPtr sort_func, const Qt::SortOrder &ascending);
    // For RomDataContainer::compare_regex
    void sort_data(const QRegularExpression &expr, const Qt::SortOrder &ascending, const SortMode &sort_mode);
    void sort_data_reverse();
    static bool compare_year(const RomData &a, const RomData &b);
    static bool compare_favorite(const RomData &a, const RomData &b);
    static bool compare_alphabet(const RomData &a, const RomData &b);
    static bool compare_regex(const RomData &a, const RomData &b, const QRegularExpression &expr, const SortMode &mode);
    
private:
    // Easy access to every single rom
    RomDataQList m_raw_data;

    // Formatted list of ptrs of data with the nested QLists acting as pages
    MimicRomDataQList m_mimic_data;

    // Should be called whenever changes have been made to the raw data
    void update_mimic_data();
};

namespace ComparePtrs {
    const constexpr CompareFuncPtr CMP_YEAR(RomDataContainer::compare_year);
    const constexpr CompareFuncPtr CMP_FAV(RomDataContainer::compare_favorite);
    const constexpr CompareFuncPtr CMP_ABC(RomDataContainer::compare_alphabet);
}