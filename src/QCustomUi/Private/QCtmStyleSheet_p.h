/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2025 LiLong                                              **
**  This file is part of QCustomUi.                                             **
**                                                                              **
**  QCustomUi is free software: you can redistribute it and/or modify           **
**  it under the terms of the GNU Lesser General Public License as published by **
**  the Free Software Foundation, either version 3 of the License, or           **
**  (at your option) any later version.                                         **
**                                                                              **
**  QCustomUi is distributed in the hope that it will be useful,                **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of              **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               **
**  GNU Lesser General Public License for more details.                         **
**                                                                              **
**  You should have received a copy of the GNU Lesser General Public License    **
**  along with QCustomUi.  If not, see <https://www.gnu.org/licenses/>.         **
**********************************************************************************/
#pragma once

#include <tao/pegtl.hpp>

#include <QStringList>

#include <optional>

struct CssRule
{
    QStringList selectors;
    std::pair<QString, QString> properties;
};

struct CssContext
{
    std::optional<CssRule> globalRule;
    std::vector<CssRule> rules;
    CssRule* current { nullptr };
};

namespace PEG
{
using namespace tao::pegtl;
using IdentifierFirst = ranges<peek_char, 'a', 'z', 'A', 'Z', '_', '-'>;
using IdentifierOther = ranges<peek_char, 'a', 'z', 'A', 'Z', '0', '9', '_', '-'>;
using Identifier      = seq<IdentifierFirst, star<IdentifierOther>>;
// clang-format off
// Selector
struct UniversalSelector : one<'*'> {};
struct TypeSelector : Identifier {};
struct ClassSelector : seq<one<'.'>, IdentifierFirst> {};
struct IDSelector : seq<opt<TypeSelector, ClassSelector>, one<'#'>, IdentifierFirst> {};
struct DescendantSelector : star<sor<TypeSelector, ClassSelector, IDSelector, star<ascii::space>>> {};
struct ChildSelector : seq<sor<TypeSelector, ClassSelector>, star<ascii::space>, one<'>'>, star<ascii::space>, sor<TypeSelector, ClassSelector>> {};
struct RootSelector : TAO_PEGTL_STRING(":root") {};
struct PropertySelector : seq<sor<RootSelector, ChildSelector, DescendantSelector, IDSelector, ClassSelector, TypeSelector, UniversalSelector>,
    one<'['>, Identifier, opt<seq<sor<one<'='>, opt<string<'~', '='>>, one<'"'>, sor<Identifier, plus<ascii::alnum>>, one<'"'>>>>, one<']'>> {};

struct Selectors : sor<PropertySelector, ChildSelector, DescendantSelector, IDSelector, ClassSelector, TypeSelector, UniversalSelector, RootSelector> {};

struct SubControl : seq<two<':'>, Identifier> {};
struct PseudoState : seq<one<':'>, opt<one<'!'>>, Identifier> {};

struct VariantKey : seq<two<'-'>, Identifier> {};
struct PropertyKey : sor<VariantKey, Identifier> {};

struct VarKey : TAO_PEGTL_STRING("var") {};
struct VarFunc : seq<VarKey, one<'('>, opt<ascii::space>, VariantKey, opt<ascii::space>, one<')'>> {};

struct PropertyValue : until<one<';', '}'>> {};

struct Property : seq<PropertyKey, star<ascii::space>, one<':'>, PropertyValue, opt<one<';'>>> {};
struct Properties : star<Property, star<ascii::space>> {};

struct RuleSelector : seq<Selectors, opt<SubControl>, star<PseudoState>> {};
struct RuleSet : seq<RuleSelector, star<ascii::space>, star<one<','>, star<ascii::space>, RuleSelector>, star<ascii::space>, one<'{'>, star<ascii::space>, Properties, one<'}'>> {};

struct RuleSets : seq<star<ascii::space>, star<RuleSet, star<ascii::space>>> {};

template <typename T>
struct Action {};
// clang-format on

template<>
struct Action<RuleSelector>
{
    template<typename ParseInput>
    void apply(ParseInput& input, CssContext& context)
    {
        std::string str(input.begin(), input.end());
        if (str == ":root")
        {
            context.globalRule = CssRule { ":root" };
            context.current    = &context.globalRule.value();
        }
    }
};
} // namespace PEG