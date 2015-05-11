/*
*    Copyright (C) 2015 MongoDB Inc.
*
*    This program is free software: you can redistribute it and/or  modify
*    it under the terms of the GNU Affero General Public License, version 3,
*    as published by the Free Software Foundation.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*    As a special exception, the copyright holders give permission to link the
*    code of portions of this program with the OpenSSL library under certain
*    conditions as described in each individual source file and distribute
*    linked combinations including the program with the OpenSSL library. You
*    must comply with the GNU Affero General Public License in all respects for
*    all of the code used other than as permitted herein. If you modify file(s)
*    with this exception, you may extend this exception to your version of the
*    file(s), but you are not obligated to do so. If you do not wish to do so,
*    delete this exception statement from your version. If you delete this
*    exception statement from all source files in the program, then also delete
*    it in the license file.
*/

#pragma once

#include "mongo/base/disallow_copying.h"
#include "mongo/base/string_data.h"
#include "mongo/db/operation_context.h"

namespace mongo {
    /**
     * If true, Collection should do no validation of writes from this OperationContext.
     *
     * Note that Decorations are value-constructed so this defaults to false.
     */
    extern const OperationContext::Decoration<bool> documentValidationDisabled;

    inline StringData bypassDocumentValidationCommandOption() {
        return "bypassDocumentValidation";
    }

    inline bool shouldBypassDocumentValidationforCommand(const BSONObj& cmdObj) {
        return cmdObj[bypassDocumentValidationCommandOption()].trueValue();
    }

    /**
     * Disables document validation on a single OperationContext while in scope.
     * Resets to original value when leaving scope so they are safe to nest.
     */
    class DisableDocumentValidation {
        MONGO_DISALLOW_COPYING(DisableDocumentValidation);
    public:
        DisableDocumentValidation(OperationContext* txn)
                : _txn(txn)
                , _initialState(documentValidationDisabled(_txn)) {
            documentValidationDisabled(_txn) = true;
        }

        ~DisableDocumentValidation() {
            documentValidationDisabled(_txn) = _initialState;
        }

    private:
        OperationContext* const _txn;
        const bool _initialState;
    };
}
