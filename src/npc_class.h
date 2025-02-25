#pragma once
#ifndef CATA_SRC_NPC_CLASS_H
#define CATA_SRC_NPC_CLASS_H

#include <functional>
#include <iosfwd>
#include <map>
#include <vector>

#include "translations.h"
#include "type_id.h"

class JsonObject;
class Trait_group;

namespace trait_group
{

using Trait_group_tag = string_id<Trait_group>;

} // namespace trait_group

// TODO: Move to better suited file (rng.h/.cpp?)
class distribution
{
    private:
        std::function<float()> generator_function;
        explicit distribution( const std::function<float()> &gen );

    public:
        distribution();
        distribution( const distribution & );

        float roll() const;

        distribution operator+( const distribution &other ) const;
        distribution operator*( const distribution &other ) const;
        distribution &operator=( const distribution &other );

        static distribution constant( float val );
        static distribution rng_roll( int from, int to );
        static distribution dice_roll( int sides, int size );
        static distribution one_in( float in );
};

struct shopkeeper_item_group {
    item_group_id id;
    int trust;
    bool strict;

    shopkeeper_item_group() : id( item_group_id( "EMPTY_GROUP" ) ), trust( 0 ), strict( false ) {}
    shopkeeper_item_group( const std::string &id, int trust, bool strict ) :
        id( item_group_id( id ) ), trust( trust ), strict( strict ) {}

    void deserialize( const JsonObject &jo );
};

class npc_class
{
    private:
        translation name;
        translation job_description;

        bool common = true;

        distribution bonus_str;
        distribution bonus_dex;
        distribution bonus_int;
        distribution bonus_per;

        std::map<skill_id, distribution> skills;
        // Just for finalization
        std::map<skill_id, distribution> bonus_skills;

        // first -> item group, second -> trust
        std::vector<shopkeeper_item_group> shop_item_groups;

    public:
        npc_class_id id;
        std::vector<std::pair<npc_class_id, mod_id>> src;
        bool was_loaded = false;

        item_group_id worn_override;
        item_group_id carry_override;
        item_group_id weapon_override;

        std::map<mutation_category_id, distribution> mutation_rounds;
        trait_group::Trait_group_tag traits = trait_group::Trait_group_tag( "EMPTY_GROUP" );
        // the int is what level the spell starts at
        std::map<spell_id, int> _starting_spells;
        std::map<bionic_id, int> bionic_list;
        std::vector<proficiency_id> _starting_proficiencies;
        npc_class();

        std::string get_name() const;
        std::string get_job_description() const;

        int roll_strength() const;
        int roll_dexterity() const;
        int roll_intelligence() const;
        int roll_perception() const;

        int roll_skill( const skill_id & ) const;

        const std::vector<shopkeeper_item_group> &get_shopkeeper_items() const;

        void load( const JsonObject &jo, const std::string &src );

        static const npc_class_id &from_legacy_int( int i );

        static const npc_class_id &random_common();

        static void load_npc_class( const JsonObject &jo, const std::string &src );

        static const std::vector<npc_class> &get_all();

        static void reset_npc_classes();

        static void finalize_all();

        static void check_consistency();
};

// TODO: Get rid of that
extern const npc_class_id NC_NONE;
extern const npc_class_id NC_EVAC_SHOPKEEP;
extern const npc_class_id NC_SHOPKEEP;
extern const npc_class_id NC_HACKER;
extern const npc_class_id NC_CYBORG;
extern const npc_class_id NC_DOCTOR;
extern const npc_class_id NC_TRADER;
extern const npc_class_id NC_NINJA;
extern const npc_class_id NC_COWBOY;
extern const npc_class_id NC_SCIENTIST;
extern const npc_class_id NC_BOUNTY_HUNTER;
extern const npc_class_id NC_THUG;
extern const npc_class_id NC_SCAVENGER;
extern const npc_class_id NC_ARSONIST;
extern const npc_class_id NC_HUNTER;
extern const npc_class_id NC_SOLDIER;
extern const npc_class_id NC_BARTENDER;
extern const npc_class_id NC_JUNK_SHOPKEEP;
extern const npc_class_id NC_HALLU;

#endif // CATA_SRC_NPC_CLASS_H
