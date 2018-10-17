// Copyright 2018 The Beam Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "wallet/common.h"
#include "wallet/wallet_db.h"

#include <boost/optional.hpp>
#include "utility/logger.h"

namespace beam { namespace wallet
{
    struct ITransaction
    {
        using Ptr = std::shared_ptr<ITransaction>;
        virtual TxType GetType() const = 0;
        virtual void Update() = 0;
        virtual void Cancel() = 0;
    };

    class TransactionFailedException : public std::runtime_error
    {
    public:
        TransactionFailedException(bool notify, const char* message = "");
        bool ShouldNofify() const;
    private:
        bool m_Notify;
    };

    //
    // State machine for managing per transaction negotiations between wallets
    // 
    class BaseTransaction : public ITransaction
    {
    public:
        using Ptr = std::shared_ptr<BaseTransaction>;
        BaseTransaction(INegotiatorGateway& gateway
                      , beam::IKeyChain::Ptr keychain
                      , const TxID& txID);

        const TxID& GetTxID() const;
        void Update() override;
        void Cancel() override;

        template <typename T>
        bool GetParameter(TxParameterID paramID, T& value) const
        {
            return getTxParameter(m_Keychain, GetTxID(), paramID, value);
        }

        template <typename T>
        void GetMandatoryParameter(TxParameterID paramID, T& value) const
        {
            if (!getTxParameter(m_Keychain, GetTxID(), paramID, value))
            {
                std::stringstream ss;
                ss << " Failed to get parameter: " << static_cast<uint8_t>(paramID);
                throw TransactionFailedException(true, ss.str().c_str());
            }
        }

        template <typename T>
        bool SetParameter(TxParameterID paramID, const T& value)
        {
            return setTxParameter(m_Keychain, GetTxID(), paramID, value);
        }

    protected:
        bool IsInitiator() const;
        void ConfirmKernel(const TxKernel& kernel);
        void CompleteTx();
        void RollbackTx();
        void UpdateTxDescription(TxStatus s);

        std::vector<Input::Ptr> GetTxInputs(const TxID& txID) const;
        std::vector<Output::Ptr> GetTxOutputs(const TxID& txID) const;
        std::vector<Coin> GetUnconfirmedOutputs() const;
        void CreateOutput(Amount amount, Height currentHeight);
        void PrepareSenderUTXOs(Amount amount, Height currentHeight);

        void OnFailed(const std::string& message = "", bool notify = false);

        bool GetTip(Block::SystemState::Full& state) const;

        bool SendTxParameters(SetTxParameter&& msg) const;
        virtual void UpdateImpl() = 0;
    protected:

        INegotiatorGateway& m_Gateway;
        beam::IKeyChain::Ptr m_Keychain;

        TxID m_ID;
        mutable boost::optional<bool> m_IsInitiator;

    };

    class SimpleTransaction : public BaseTransaction
    {
    public:
        SimpleTransaction(INegotiatorGateway& gateway
                        , beam::IKeyChain::Ptr keychain
                        , const TxID& txID);
    private:
        TxType GetType() const override;
        void UpdateImpl() override;
    };

    struct SignatureBuilder
    {
        BaseTransaction& m_Tx;
        TxKernel::Ptr m_Kernel;

        ECC::Scalar::Native m_BlindingExcess;
        ECC::Scalar::Native m_PeerSignature;
        ECC::Scalar::Native m_PartialSignature;
        ECC::Point::Native m_PublicPeerNonce;
        ECC::Point::Native m_PublicPeerExcess;
        ECC::Point::Native m_PublicNonce;
        ECC::Point::Native m_PublicExcess;
        ECC::Hash::Value m_Message;
        ECC::Signature::MultiSig m_MultiSig;

        SignatureBuilder(BaseTransaction& tx);

        void CreateKernel(Amount fee, Height minHeight);
        void SetBlindingExcess(const ECC::Scalar::Native& blindingExcess);
        bool ApplyBlindingExcess();
        bool ApplyPublicPeerNonce();
        bool ApplyPublicPeerExcess();
        void SignPartial();
        bool ApplyPeerSignature();
        bool IsValidPeerSignature() const;
        void FinalizeSignature();
    };
}}