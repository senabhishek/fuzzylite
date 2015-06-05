/*
 Copyright © 2010-2015 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#ifndef FL_GENERAL_H
#define	FL_GENERAL_H

#include "fl/fuzzylite.h"

#include "fl/activation/Activation.h"

namespace fl {

    /**
     * General activates every rule in any given {@link RuleBlock} following the
     * order in which the rules were added to the rule block
     * 
     * @author Juan Rada-Vilela, Ph.D.
     * @see Rule
     * @see RuleBlock
     * @see ActivationFactory
     * @since 6.0
     */

    class FL_API General : public Activation {
    public:

        General();
        virtual ~General();
        FL_DEFAULT_COPY_AND_MOVE(General)

        virtual std::string className() const FL_IOVERRIDE;

        /**
         * No parameters are required to configure the activation method
         * 
         * @return an empty string
         */
        virtual std::string parameters() const FL_IOVERRIDE;

        /**
         * No parameters are required to configure the activation method
         * 
         * @param parameters is an empty string
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        /**
         * Activates every rule in the given {@link RuleBlock} following 
         * the order in which the rules were added to the rule block
         * 
         * @param ruleBlock is the rule block to activate
         */
        virtual void activate(RuleBlock* ruleBlock) const FL_IOVERRIDE;

        virtual General* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };

}

#endif	/* FL_GENERAL_H */