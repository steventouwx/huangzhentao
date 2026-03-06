#ifndef RMC_RCS_CAPABILITY_RULE_H
#define RMC_RCS_CAPABILITY_RULE_H

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

namespace vendor::rmc::rcs {
    const int BASE_PADDING = 4;

    inline int32_t getSizeWPad(int32_t size) {
        return (size + (BASE_PADDING - 1)) & (~(BASE_PADDING-1));
    }

    typedef struct _RuleCond final {
        int32_t m_length;
        int32_t m_header;
        char   *m_param;
        struct _RuleCond *m_next;

        _RuleCond(int hdr, const char *para) {
            m_header = hdr;
            if (para) {
                m_length = getSizeWPad((strlen(para)+1));
            } else {
                m_length = 0;
            }
            if (m_length > 0) {
                m_param = (char *)malloc(m_length);
                memcpy(m_param, para, strlen(para)+1);
            }
            m_next = 0;
        }

        ~_RuleCond() {
            if (m_param) {
                free(m_param);
            }
        }

        /**
         *  @brief calculate rule_cond for continuous memory allocation
         *  @note  size contains "len_of_param(4)+header(4)+param(n)",
         *         not include param, next
         */
        int32_t getSize() const {
            return sizeof(m_length) + sizeof(m_header) + m_length;
        }

        int packAll(uint8_t *buffer, int32_t bufSize);

    } RuleCondition;

    typedef struct _RuleItem final {
        int32_t m_length;
        int32_t m_cond_num;
        struct _RuleCond *m_conds;
        struct _RuleItem *m_next;

        _RuleItem() {
            m_length = 0;
            m_cond_num = 0;
            m_conds = 0;
            m_next = 0;
        }

        ~_RuleItem() {
            RuleCondition *current = 0, *next = 0;

            for (current = m_conds; current; current = next) {
                next = current->m_next;
                delete current;
            }
        }

        /**
         *  @brief calculate ruleItem for continuous memory allocation
         *  @note  size contains "len_of_param(4)+header(4)+param(n)",
         *         not include conds, next
         */
        int32_t getSize() const {
            return sizeof(m_length) + sizeof(m_cond_num) + m_length;
        }

        int add(RuleCondition *cond);
        RuleCondition *getCondition(int hdr);
        int packAll(uint8_t *buffer, int32_t bufSize);

    } RuleItem;


    typedef struct _RuleSet {
        int32_t m_length;
        int32_t m_item_num;
        int32_t m_method;

        struct _RuleItem *m_items;
        struct _RuleSet  *m_next;

        _RuleSet(int method) {
            m_method = method;
            m_length = 0;
            m_item_num = 0;
            m_items = 0;
            m_next = 0;
        }

        ~_RuleSet() {
            RuleItem *current = 0, *next = 0;

            for (current = m_items; current; current = next) {
                next = current->m_next;
                delete current;
            }
        }

        int32_t getSize() const {
            return sizeof(m_length)
                    + sizeof(m_item_num)
                    + sizeof(m_method)
                    + m_length;
        }

        int add(RuleItem *item);
        int packAll(uint8_t *buffer, int32_t bufSize);

    } RuleSet;


    typedef struct _RuleLevel2 {
        int32_t m_length;
        int32_t m_set_num;
        struct _RuleSet *m_sets;

        _RuleLevel2() {
            m_length = 0;
            m_set_num = 0;
            m_sets = 0;
        }

        ~_RuleLevel2() {
            RuleSet *current = 0, *next = 0;

            for (current = m_sets; current; current = next) {
                next = current->m_next;
                delete current;
            }
        }

        int32_t getSize() const {
            return sizeof(m_length) + sizeof(m_set_num) + m_length;
        }

        int add(RuleSet *set);
        RuleSet *findRuleSet(int method);
        int packAll(uint8_t *buffer, int32_t bufSize);
    } RuleLevel2;


    class RcsCapabilityRule final {
        public:
            RcsCapabilityRule();
            ~RcsCapabilityRule();

            int setLevel0(char *userInfo, const char *associated);
            int setLevel1(const char *featureSet);
            int addLevel2(int method, int header, const char *field);
            int setLevel3(const char *body);
            int packAll();

            uint8_t *getPackedData() const {
                return m_packed_data;
            }

        private:
            char *m_user_info;
            char *m_associated;
            char *m_feature_set;
            char *m_body_content;
            RuleLevel2 *m_level2;
            uint8_t *m_packed_data;
    };
} //namespace: rmc

#endif

