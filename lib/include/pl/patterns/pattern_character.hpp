#pragma once

#include <pl/patterns/pattern.hpp>

namespace pl::ptrn {

    class PatternCharacter : public Pattern {
    public:
        explicit PatternCharacter(core::Evaluator *evaluator, u64 offset)
            : Pattern(evaluator, offset, 1) { }

        [[nodiscard]] std::unique_ptr<Pattern> clone() const override {
            return std::unique_ptr<Pattern>(new PatternCharacter(*this));
        }

        [[nodiscard]] core::Token::Literal getValue() const override {
            char character = '\x00';
            this->getEvaluator()->readData(this->getOffset(), &character, 1, this->getSection());

            return character;
        }

        std::vector<u8> getBytesOf(const core::Token::Literal &value) const override {
            if (auto charValue = std::get_if<char>(&value); charValue != nullptr)
                return hlp::toBytes(*charValue);
            else
                return { };
        }

        [[nodiscard]] std::string getFormattedName() const override {
            return "char";
        }

        std::string getFormattedValue() override {
            const u8 character = core::Token::literalToCharacter(this->getValue());
            return this->formatDisplayValue(fmt::format("'{0}'", hlp::encodeByteString({ character })), this->getValue());
        }

        [[nodiscard]] bool operator==(const Pattern &other) const override { return areCommonPropertiesEqual<decltype(*this)>(other); }

        void accept(PatternVisitor &v) override {
            v.visit(*this);
        }

        [[nodiscard]] std::string toString() const override {
            auto value = this->getValue();
            auto result = fmt::format("{}", hlp::encodeByteString({ u8(core::Token::literalToCharacter(value)) }));

            return this->formatDisplayValue(result, value);
        }
    };

}