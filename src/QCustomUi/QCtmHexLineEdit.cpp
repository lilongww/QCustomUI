/*********************************************************************************
**                                                                              **
**  Copyright (C) 2019-2024 LiLong                                              **
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
#include "QCtmHexLineEdit.h"

#include <QValidator>
#include <tao/pegtl.hpp>

namespace PEG
{
struct Context
{
    std::string output;
    std::string byteGroup;
    inline void pushByte()
    {
        if (!output.empty())
            output.push_back(' ');
        output += byteGroup;
        byteGroup.clear();
    }
    inline bool format()
    {
        if (!byteGroup.empty())
        {
            if (!output.empty())
                output.push_back(' ');
            output.push_back(byteGroup.front());
            return true;
        }
        return false;
    }
    inline void fixup()
    {
        if (!byteGroup.empty()) // 单字符
        {
            output.push_back(' ');
            output.push_back('0');
            output.push_back(byteGroup.front());
        }
    }
};
using namespace tao::pegtl;
using Char       = ascii::xdigit;
using Expression = star<sor<Char, ascii::space, ascii::any>>;

template<typename T>
struct Action
{
};

template<>
struct Action<Char>
{
    template<typename ParseInput>
    static void apply(const ParseInput& in, Context& ctx)
    {
        ctx.byteGroup.push_back(*in.begin());
        if (ctx.byteGroup.size() == 2)
        {
            ctx.pushByte();
        }
    }
};
} // namespace PEG

class QCUSTOMUI_EXPORT QCtmHexValidator : public QValidator
{
public:
    using QValidator::QValidator;
    State validate(QString&, int&) const override;
    void fixup(QString&) const override;
};

QValidator::State QCtmHexValidator::validate(QString& input, int& pos) const
{
    PEG::Context ctx;
    auto str = input.toStdString();
    tao::pegtl::parse<PEG::Expression, PEG::Action>(tao::pegtl::string_input(str, ""), ctx);
    auto ret = ctx.format();
    input    = QString::fromStdString(ctx.output);
    pos      = input.size();
    return ret ? QValidator::State::Intermediate : QValidator::State::Acceptable;
}

void QCtmHexValidator::fixup(QString& input) const
{
    PEG::Context ctx;
    tao::pegtl::parse<PEG::Expression, PEG::Action>(tao::pegtl::string_input(input.toStdString(), ""), ctx);
    ctx.fixup();
    input = QString::fromStdString(ctx.output);
}
/*!
    \class      QCtmHexLineEdit
    \brief      16进制单行编辑框.
    \inherits   QLineEdit
    \ingroup    QCustomUi
    \inmodule   QCustomUi
    \inheaderfile QCtmHexLineEdit.h
*/

/*!
    \brief      构造函数 \a parent.
*/
QCtmHexLineEdit::QCtmHexLineEdit(QWidget* parent) : QLineEdit(parent) { setValidator(new QCtmHexValidator(this)); }

/*!
    \brief      构造函数 \a text, \a parent.
*/
QCtmHexLineEdit::QCtmHexLineEdit(const QString& text, QWidget* parent /*= nullptr*/) : QLineEdit(text, parent)
{
    setValidator(new QCtmHexValidator(this));
}

/*!
    \brief      析构函数.
*/
QCtmHexLineEdit::~QCtmHexLineEdit() {}
