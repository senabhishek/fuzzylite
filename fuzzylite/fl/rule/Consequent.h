/*
 * Consequent.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_CONSEQUENT_H
#define FL_CONSEQUENT_H

#include "fl/scalar.h"

#include <string>

namespace fl {
    class TNorm;

    class Consequent {
    public:

        Consequent() { }

        virtual ~Consequent() { }

        virtual void fire(scalar strength, const TNorm* activation) = 0;

        virtual std::string toString() const = 0;

    };

}

#endif /* FL_CONSEQUENT_H */
