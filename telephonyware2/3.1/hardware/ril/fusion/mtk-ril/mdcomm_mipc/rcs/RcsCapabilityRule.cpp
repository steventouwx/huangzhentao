#include "mtk_log.h"
#include "RcsCapabilityRule.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

namespace vendor::rmm::rcs {
    static const char *RULE_LOG_TAG = (const char *)"RcsCapRule";

    int RuleCondition::packAll(uint8_t *buffer, int32_t bufSize) {
        RuleCondition *current = this;
        int length = 0, hdrSize = sizeof(m_length)+sizeof(m_header);

        if (!buffer)
            return -1;

        for (; current; current = current->m_next) {
            memcpy(&buffer[length], (void *)current, hdrSize);
            memcpy(&buffer[length+hdrSize], current->m_param, current->m_length);

            length += current->getSize();
            if (length > bufSize) {
                mtkLogE(RULE_LOG_TAG, "RuleCondition::packAll->insufficient memory space, size:%d, rule_cond len:%d",
                        bufSize, length);
                return -1;
            }
        }

        return length;
    }

    int RuleItem::add(RuleCondition *cond) {
        RuleCondition *current = 0, *prev = 0;
        if (!cond)
            return -1;

        if (m_cond_num) {
            current = prev = m_conds;

            while (current) {
                prev = current;
                current = current->m_next;
            }

            prev->m_next = cond;
            cond->m_next = 0;
        } else {
            m_conds = cond;
        }

        m_cond_num++;
        m_length += cond->getSize();

        return 0;
    }

    int RuleItem::packAll(uint8_t *buffer, int32_t bufSize) {
        RuleItem *current = this;
        int length = 0, hdrSize = sizeof(m_length) + sizeof(m_cond_num);

        if (!buffer || bufSize <= 0)
            return -1;

        for (; current; current = current->m_next) {
            memcpy(&buffer[length], (void *)current, hdrSize);
            if (current->m_conds) {
                current->m_conds->packAll(&buffer[length+hdrSize], bufSize-length-hdrSize);
            }
            length += current->getSize();
            if (length > bufSize) {
                mtkLogE(RULE_LOG_TAG, "RuleItem::packAll->insufficient memory space, size:%d, rule_item length:%d",
                        bufSize, length);
                return -1;
            }
        }

        return length;
    }


    RuleCondition *RuleItem::getCondition(int hdr) {
        RuleCondition *current = 0;

        for (current = m_conds; current; current = current->m_next) {
            if (hdr == current->m_header)
                break;
        }

        return current;
    }

    int RuleSet::add(RuleItem *item) {
        RuleItem *current = 0, *prev = 0;

        if (!item)
            return -1;

        if (m_item_num > 0) {
            for (current = prev = m_items; current; current = current->m_next) {
                prev = current;
            }

            prev->m_next = item;
        } else {
            m_items = item;
        }

        m_item_num++;
        m_length += item->getSize();

        return 0;
    }

    int RuleSet::packAll(uint8_t *buffer, int32_t bufSize) {
        RuleSet *current = this;
        int length = 0, hdrSize = sizeof(m_length) + sizeof(m_item_num) + sizeof(m_method);

        if (!buffer)
            return -1;

        for (; current; current = current->m_next) {
            memcpy(&buffer[length], (void *)current, hdrSize);
            if (current->m_items) {
                current->m_items->packAll(&buffer[length+hdrSize], bufSize-length-hdrSize);
            }

            length += current->getSize();
            if (length > bufSize) {
                mtkLogE(RULE_LOG_TAG, "RuleSet::packAll->insufficient memory space, size:%d, length:%d",
                        bufSize, length);
                return -1;
            }
        }

        return length;
    }

    int RuleLevel2::add(RuleSet *set) {
        RuleSet *current = 0, *prev = 0;

        if (!set)
            return -1;

        if (m_set_num > 0) {
            for (current = prev = m_sets; current; current = current->m_next) {
                if (set->m_method == current->m_method) {
                    // replace cur by set
                    if (m_sets == current)
                        m_sets = set;
                    else
                        prev->m_next = set;

                    set->m_next = current->m_next;

                    // delete cur when method conflict !
                    m_set_num--;
                    m_length -= current->getSize();
                    delete current;
                    current = set;
                    break;
                }
                prev = current;
            }

            // append to last
            if (!current) {
                prev->m_next = set;
            }
        } else {
            m_sets = set;
        }

        // update num_of_set, len_of_level_2
        m_set_num++;
        m_length += set->getSize();

        return 0;
    }

    RuleSet *RuleLevel2::findRuleSet(int method) {
        RuleSet *current = 0;

        for (current = m_sets; current; current = current->m_next) {
            if (current->m_method == method)
                break;
        }

        return current;
    }

    int RuleLevel2::packAll(uint8_t *buffer, int32_t bufSize) {
        int hdrSize = sizeof(m_length) + sizeof(m_set_num);
        int length = hdrSize;

        if (!buffer)
            return -1;

        if (bufSize < getSize()) {
            mtkLogE(RULE_LOG_TAG, "RuleLevel2::packAll->insufficient memory space, size:%d, level2 total:%d",
                    bufSize, getSize());
            return -1;
        }

        // pack lv2's "len_of_level_2", "num_of_set"
        memcpy(buffer, (void *)this, hdrSize);
        if (m_sets) {
            length += m_sets->packAll(&buffer[hdrSize], bufSize-hdrSize);
        }

        return length;
    }

    RcsCapabilityRule::RcsCapabilityRule() {
        m_user_info = 0;
        m_associated = 0;
        m_feature_set = 0;
        m_body_content = 0;
        m_level2 = 0;
        m_packed_data = 0;
    }

    RcsCapabilityRule::~RcsCapabilityRule() {
        if (m_level2) {
            delete m_level2;
        }
        if (m_user_info) {
            free(m_user_info);
        }
        if (m_associated) {
            free(m_associated);
        }
        if (m_feature_set) {
            free(m_feature_set);
        }
        if (m_body_content) {
            free(m_body_content);
        }
        if (m_packed_data) {
            free(m_packed_data);
        }
    }

    int RcsCapabilityRule::setLevel0(char *userInfo, const char *associated) {
        char *ptr = 0;
        int length = 0;

        if (userInfo) {
            ptr = strchr(userInfo, '@');
            if (ptr) {
                length = ptr - userInfo;
            } else {
                length = strlen(userInfo);
            }
            m_user_info = (char *)malloc(length+1);
            memcpy(m_user_info, userInfo, length);
            m_user_info[length] = '\0';
        }

        if (associated) {
            m_associated = strdup(associated);
        }

        return 0;
    }

    int RcsCapabilityRule::setLevel1(const char *featureSet) {
        if (featureSet) {
            m_feature_set = strdup(featureSet);
        }

        return 0;
    }

    int RcsCapabilityRule::addLevel2(int method, int header, const char *field) {
        if (!m_level2) {
            m_level2 = new RuleLevel2();
        }

        RuleSet  *set  = 0;
        RuleCondition *cond = new RuleCondition(header, field);
        int append = 0;

        if (!cond) return -1;

        RuleItem *item = new RuleItem();
        if (!item) {
            delete cond;
            return -1;
        }

        item->add(cond);
        append = item->getSize();

        set = m_level2->findRuleSet(method);
        if (set) {
            set->add(item);
            m_level2->m_length += append;
        } else {
            set = new RuleSet(method);
            if (!set) {
                delete item;
                return -1;
            }
            set->add(item);
            m_level2->add(set);
        }

        return 0;
    }

    int RcsCapabilityRule::setLevel3(const char *body) {
        if (!body)
            return -1;

        m_body_content = strdup(body);

        return 0;
    }

    int RcsCapabilityRule::packAll() {
        int32_t total = 0, l = 0;
        int result = 0;

        if (!m_user_info || 0 == strlen(m_user_info)) {
            return -1;
        }

        // | <-- level 0 --->| <-- level 0 --->| <- level 1 -->| <- level 2 -->|
        //  ----- ----------- ----- ----------- ----- --------- ---------- ----
        // | len | user_info | len | assoc_set | len | ftr_set | lv2 ....      |
        //  ----- ----------- ----- ----------- ----- --------- ---------- ----
        l = getSizeWPad(strlen(m_user_info));
        total = sizeof(int32_t) + l;

        total += sizeof(int32_t);
        if (m_associated) {
            l = getSizeWPad(strlen(m_associated));
            total += l;
        }

        total += sizeof(int32_t);
        if (m_feature_set) {
            l = getSizeWPad(strlen(m_feature_set));
            total += l;
        }

        total += sizeof(int32_t);
        if (m_body_content) {
            l = getSizeWPad(strlen(m_body_content));
            total += l;
        }

        if (m_level2) {
            total += m_level2->getSize();
        }

        // packing
        m_packed_data = (uint8_t *)malloc(total);
        if (!m_packed_data)
            return -1;
        memset(m_packed_data, 0, total);
        uint8_t *write = m_packed_data;

        // lv0, user_info
        l = getSizeWPad(strlen(m_user_info));
        *((int32_t *)write) = l;
        write += sizeof(int32_t);
        memcpy(write, m_user_info, strlen(m_user_info));
        write += l;

        // lv0, assoc_set
        if (m_associated) {
            l = getSizeWPad(strlen(m_associated));
            *((int32_t *)write) = l;
            write += sizeof(int32_t);
            memcpy(write, m_associated, strlen(m_associated));
            write += l;
        } else {
            *((int32_t *)write) = 0;
            write += sizeof(int32_t);
        }

        // lv1, ftr_set
        if (m_feature_set) {
            l = getSizeWPad(strlen(m_feature_set));
            *((int32_t *)write) = l;
            write += sizeof(int32_t);
            memcpy(write, m_feature_set, strlen(m_feature_set));
            write += l;
        } else {
            *((int32_t *)write) = 0;
            write += sizeof(int32_t);
        }


        // lv3, body_str
        if (m_body_content) {
            l = getSizeWPad(strlen(m_body_content));
            *((int32_t *)write) = l;
            write += sizeof(int32_t);
            memcpy(write, m_body_content, strlen(m_body_content));
            write += l;
        } else {
            *((int32_t *)write) = 0;
            write += sizeof(int32_t);
        }

        if (m_level2) {
            result = m_level2->packAll(write, total - (write - m_packed_data));
        }

        return result;
    }
} // namespace: rmm
