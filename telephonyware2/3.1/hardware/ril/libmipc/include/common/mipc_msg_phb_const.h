#ifndef __MIPC_MSG_PHB_CONST_H__
#define __MIPC_MSG_PHB_CONST_H__

enum MIPC_PHB_MSG_enum {
    MIPC_PHB_MSG_NONE = 0,
    /* edit or delete upb entry */
    MIPC_PHB_SET_UPB_ENTRY_REQ                              = 3072,
    MIPC_PHB_SET_UPB_ENTRY_CNF                              = 3073,

    /* read upb entry */
    MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ                = 3074,
    MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF                = 3075,

    /* read upb entry */
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_REQ             = 3076,
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF             = 3077,

    /* get phb memory storage info */
    MIPC_PHB_GET_PHB_STORAGE_INFO_REQ                       = 3078,
    MIPC_PHB_GET_PHB_STORAGE_INFO_CNF                       = 3079,

    /* get phb memory storage info */
    MIPC_PHB_SET_PHB_MEM_STORAGE_REQ                        = 3080,
    MIPC_PHB_SET_PHB_MEM_STORAGE_CNF                        = 3081,

    /* get phb entry from bindex to eindex */
    MIPC_PHB_GET_PHB_ENTRY_REQ                              = 3082,
    MIPC_PHB_GET_PHB_ENTRY_CNF                              = 3083,

    /* set phb entry */
    MIPC_PHB_SET_PHB_ENTRY_REQ                              = 3084,
    MIPC_PHB_SET_PHB_ENTRY_CNF                              = 3085,

    /* get phb string length req */
    MIPC_PHB_GET_PHB_STRINGSLENGTH_REQ                      = 3086,
    MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF                      = 3087,

    /* get upb capability req */
    MIPC_PHB_GET_UPB_CAPABILITY_REQ                         = 3088,
    MIPC_PHB_GET_UPB_CAPABILITY_CNF                         = 3089,

    /* get phb string length req */
    MIPC_PHB_GET_PHB_AVAILABLE_REQ                          = 3090,
    MIPC_PHB_GET_PHB_AVAILABLE_CNF                          = 3091,

    /* PHB ready state indication */
    MIPC_PHB_READY_STATE_IND                                = 19456,


};

    /* MIPC_MSG.PHB_SET_UPB_ENTRY_REQ */
enum mipc_phb_set_upb_entry_req_tlv_enum {
    mipc_phb_set_upb_entry_req_tlv_NONE = 0,
    /* operation: 2 means edit,3 means delete */
    /* type = uint32_t */
    MIPC_PHB_SET_UPB_ENTRY_REQ_T_OP                         = 0x100,
    /* the type of USIM phonebook related EF files.  */
    /* type = uint32_t, refer to PHB_EF_FILE_TYPE */
    MIPC_PHB_SET_UPB_ENTRY_REQ_T_TYPE                       = 0x101,
    /* the index of ADN entry to be accessed. */
    /* type = uint32_t */
    MIPC_PHB_SET_UPB_ENTRY_REQ_T_ADN_INDEX                  = 0x102,
    /* the index of EF entry to be accessed. */
    /* type = uint32_t */
    MIPC_PHB_SET_UPB_ENTRY_REQ_T_EF_ENTRY_INDEX             = 0x103,
    /* the type of number, valid value:129(normal) 145(international); Default value is 0. */
    /* type = uint32_t */
    MIPC_PHB_SET_UPB_ENTRY_REQ_T_TON                        = 0x104,
    /* the associated EF_AAS entry index; Default value is 0. */
    /* type = uint32_t */
    MIPC_PHB_SET_UPB_ENTRY_REQ_T_AAS_ID                     = 0x105,
    /* group id list count,max is 10; Default value is 0. */
    /* type = uint32_t */
    MIPC_PHB_SET_UPB_ENTRY_REQ_T_GRP_COUNT                  = 0x106,
    /* group id list,valid value 0-255. */
    /* type = uint32_t */
    MIPC_PHB_SET_UPB_ENTRY_REQ_T_GRP_ID_LIST                = 0x107,
    /* number/email/alphaID string; Default value is null character. */
    /* type = string */
    MIPC_PHB_SET_UPB_ENTRY_REQ_T_LINE                       = 0x8108,
    /* number/email/alphaID string encode method; Default value is 0. */
    /* type = uint32_t, refer to PHB_ENCODE_METHOD */
    MIPC_PHB_SET_UPB_ENTRY_REQ_T_ENCODE_METHOD              = 0x109,
};

    /* MIPC_MSG.PHB_SET_UPB_ENTRY_CNF */
enum mipc_phb_set_upb_entry_cnf_tlv_enum {
    mipc_phb_set_upb_entry_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ */
enum mipc_phb_get_upb_anr_email_sne_entry_req_tlv_enum {
    mipc_phb_get_upb_anr_email_sne_entry_req_tlv_NONE = 0,
    /* the type of USIM phonebook related EF files. 0:EF_ANR 1:EF_EMAIL 2:EF_SNE */
    /* type = uint32_t, refer to PHB_EF_FILE_TYPE */
    MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ_T_TYPE         = 0x100,
    /* the index of ADN entry to be accessed. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ_T_ADN_INDEX    = 0x101,
    /* the index of EF entry to be accessed. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_REQ_T_EF_ENTRY_INDEX = 0x8102,
};

    /* MIPC_MSG.PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF */
enum mipc_phb_get_upb_anr_email_sne_entry_cnf_tlv_enum {
    mipc_phb_get_upb_anr_email_sne_entry_cnf_tlv_NONE = 0,
    /* phb anr entry */
    /* type = struct, refer to phb_anr_entry */
    MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF_T_PHB_ENTRY    = 0x8100,
    /* email str */
    /* type = struct, refer to phb_email_str */
    MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF_T_EMAIL        = 0x8101,
    /* sne str. */
    /* type = struct, refer to phb_name_str */
    MIPC_PHB_GET_UPB_ANR_EMAIL_SNE_ENTRY_CNF_T_SNESTR       = 0x8102,
};

    /* MIPC_MSG.PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_REQ */
enum mipc_phb_get_upb_aas_gas_grp_list_entry_req_tlv_enum {
    mipc_phb_get_upb_aas_gas_grp_list_entry_req_tlv_NONE = 0,
    /* the type of USIM phonebook related EF files. 3:EF_AAS 4:EF_GAS 5:EF_GRP */
    /* type = uint32_t, refer to PHB_EF_FILE_TYPE */
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_REQ_T_TYPE      = 0x100,
    /* the begining index of ADN entry to be accessed. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_REQ_T_BINDEX    = 0x101,
    /* the ending index of ADN entry to be accessed. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_REQ_T_EINDEX    = 0x102,
};

    /* MIPC_MSG.PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF */
enum mipc_phb_get_upb_aas_gas_grp_list_entry_cnf_tlv_enum {
    mipc_phb_get_upb_aas_gas_grp_list_entry_cnf_tlv_NONE = 0,
    /* aas entry count. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_AAS_COUNT = 0x100,
    /* aas str array. */
    /* type = struct_array, refer to phb_name_str */
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_AAS_LIST  = 0x8101,
    /* gas entry count. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_GAS_COUNT = 0x102,
    /* gas str array. */
    /* type = struct_array, refer to phb_name_str */
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_GAS_LIST  = 0x8103,
    /* grp id list count. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_GRP_COUNT = 0x104,
    /* group id list,valid value 0-255. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_GRP_ID_LIST = 0x105,
    /* aas str array. */
    /* type = struct, refer to phb_name_str */
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_AAS_TLV_ARRAY = 0x8106,
    /* gas str array. */
    /* type = struct, refer to phb_name_str */
    MIPC_PHB_GET_UPB_AAS_GAS_GRP_LIST_ENTRY_CNF_T_GAS_TLV_ARRAY = 0x8107,
};

    /* MIPC_MSG.PHB_GET_PHB_STORAGE_INFO_REQ */
enum mipc_phb_get_phb_storage_info_req_tlv_enum {
    mipc_phb_get_phb_storage_info_req_tlv_NONE = 0,
    /* the storage type of query, null means to get current storage type info; Default value is 8. */
    /* type = uint32_t, refer to PHB_STROAGE_TYPE */
    MIPC_PHB_GET_PHB_STORAGE_INFO_REQ_T_STORAGE_TYPE        = 0x100,
};

    /* MIPC_MSG.PHB_GET_PHB_STORAGE_INFO_CNF */
enum mipc_phb_get_phb_storage_info_cnf_tlv_enum {
    mipc_phb_get_phb_storage_info_cnf_tlv_NONE = 0,
    /* the used memory. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_USED                = 0x100,
    /* the total memory. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_TOTAL               = 0x101,
    /* the max length of number. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_NLENGTH             = 0x102,
    /* the max length of text. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_TLENGTH             = 0x103,
    /* the storage type of query. */
    /* type = uint32_t, refer to PHB_STROAGE_TYPE */
    MIPC_PHB_GET_PHB_STORAGE_INFO_CNF_T_STORAGE_TYPE        = 0x104,
};

    /* MIPC_MSG.PHB_SET_PHB_MEM_STORAGE_REQ */
enum mipc_phb_set_phb_mem_storage_req_tlv_enum {
    mipc_phb_set_phb_mem_storage_req_tlv_NONE = 0,
    /* the storage type. */
    /* type = uint32_t, refer to PHB_STROAGE_TYPE */
    MIPC_PHB_SET_PHB_MEM_STORAGE_REQ_T_STORAGE_TYPE         = 0x100,
    /* the password str; Default value is null character. */
    /* type = string */
    MIPC_PHB_SET_PHB_MEM_STORAGE_REQ_T_PASSWORD             = 0x8101,
};

    /* MIPC_MSG.PHB_SET_PHB_MEM_STORAGE_CNF */
enum mipc_phb_set_phb_mem_storage_cnf_tlv_enum {
    mipc_phb_set_phb_mem_storage_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.PHB_GET_PHB_ENTRY_REQ */
enum mipc_phb_get_phb_entry_req_tlv_enum {
    mipc_phb_get_phb_entry_req_tlv_NONE = 0,
    /* the index of beginning entry. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_ENTRY_REQ_T_BINDEX                     = 0x100,
    /* the index of ending entry. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_ENTRY_REQ_T_EINDEX                     = 0x101,
    /* storage type. when extended is 1,storage should be SM type; Default value is 8. */
    /* type = uint32_t, refer to PHB_STROAGE_TYPE */
    MIPC_PHB_GET_PHB_ENTRY_REQ_T_STORAGE_TYPE               = 0x102,
    /* the extended type or not, 1 means extended. */
    /* type = uint8_t */
    MIPC_PHB_GET_PHB_ENTRY_REQ_T_EXT                        = 0x103,
};

    /* MIPC_MSG.PHB_GET_PHB_ENTRY_CNF */
enum mipc_phb_get_phb_entry_cnf_tlv_enum {
    mipc_phb_get_phb_entry_cnf_tlv_NONE = 0,
    /* the number of entry count */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_ENTRY_CNF_T_ENTRY_COUNT                = 0x100,
    /* entry list */
    /* type = struct_array, refer to phb_entry */
    MIPC_PHB_GET_PHB_ENTRY_CNF_T_ENTRY_LIST                 = 0x8101,
    /* entry list */
    /* type = struct, refer to phb_entry */
    MIPC_PHB_GET_PHB_ENTRY_CNF_T_ENTRY_TLV_ARRAY            = 0x8102,
};

    /* MIPC_MSG.PHB_SET_PHB_ENTRY_REQ */
enum mipc_phb_set_phb_entry_req_tlv_enum {
    mipc_phb_set_phb_entry_req_tlv_NONE = 0,
    /* storage type,when extended is 1,storage should be SM type; Default value is 8. */
    /* type = uint32_t, refer to PHB_STROAGE_TYPE */
    MIPC_PHB_SET_PHB_ENTRY_REQ_T_TYPE                       = 0x100,
    /* the extended type or not, 1 means extended. */
    /* type = uint8_t */
    MIPC_PHB_SET_PHB_ENTRY_REQ_T_EXT                        = 0x101,
    /* the entry to write */
    /* type = struct, refer to phb_entry */
    MIPC_PHB_SET_PHB_ENTRY_REQ_T_ENTRY                      = 0x8102,
};

    /* MIPC_MSG.PHB_SET_PHB_ENTRY_CNF */
enum mipc_phb_set_phb_entry_cnf_tlv_enum {
    mipc_phb_set_phb_entry_cnf_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.PHB_GET_PHB_STRINGSLENGTH_REQ */
enum mipc_phb_get_phb_stringslength_req_tlv_enum {
    mipc_phb_get_phb_stringslength_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.PHB_GET_PHB_STRINGSLENGTH_CNF */
enum mipc_phb_get_phb_stringslength_cnf_tlv_enum {
    mipc_phb_get_phb_stringslength_cnf_tlv_NONE = 0,
    /* the max number length. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF_T_MAX_NUM_LEN        = 0x100,
    /* the max alpha string length. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF_T_MAX_ALPHA_LEN      = 0x101,
    /* the max additional number alpha string length */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF_T_MAX_AAS_LEN        = 0x102,
    /* the max group info alpha string length */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF_T_MAX_GAS_LEN        = 0x103,
    /* the max second name string length. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF_T_MAX_SNE_LEN        = 0x104,
    /* the max email string length. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_STRINGSLENGTH_CNF_T_MAX_EMAIL_LEN      = 0x105,
};

    /* MIPC_MSG.PHB_GET_UPB_CAPABILITY_REQ */
enum mipc_phb_get_upb_capability_req_tlv_enum {
    mipc_phb_get_upb_capability_req_tlv_NONE = 0,
    /* no proprietary TLV */
};

    /* MIPC_MSG.PHB_GET_UPB_CAPABILITY_CNF */
enum mipc_phb_get_upb_capability_cnf_tlv_enum {
    mipc_phb_get_upb_capability_cnf_tlv_NONE = 0,
    /* the number supported ANR for a ADN entry. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_ANR               = 0x100,
    /* the number supported EMAIL for a ADN entry. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_EMAIL             = 0x101,
    /* the number supported SNE for a ADN entry. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_SNE               = 0x102,
    /* maximum number of AAS entries. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_AAS               = 0x103,
    /* maximum length of the AAS in the entries. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_LEN_AAS               = 0x104,
    /* maximum number of GAS entries. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_GAS               = 0x105,
    /* maximum length of the GAS in the entries. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_LEN_GAS               = 0x106,
    /* maximum number of GRP entries. */
    /* type = uint32_t */
    MIPC_PHB_GET_UPB_CAPABILITY_CNF_T_NUM_GRP               = 0x107,
};

    /* MIPC_MSG.PHB_GET_PHB_AVAILABLE_REQ */
enum mipc_phb_get_phb_available_req_tlv_enum {
    mipc_phb_get_phb_available_req_tlv_NONE = 0,
    /* the type of phonebook related EF files. */
    /* type = uint32_t, refer to PHB_EF_FILE_TYPE */
    MIPC_PHB_GET_PHB_AVAILABLE_REQ_T_TYPE                   = 0x100,
    /* the index of EF file associated with an entry */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_AVAILABLE_REQ_T_INDEX                  = 0x101,
};

    /* MIPC_MSG.PHB_GET_PHB_AVAILABLE_CNF */
enum mipc_phb_get_phb_available_cnf_tlv_enum {
    mipc_phb_get_phb_available_cnf_tlv_NONE = 0,
    /* max number of entries in the queried EF files. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_AVAILABLE_CNF_T_MAX_NUM                = 0x100,
    /* max number of available entries in the queried EF files. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_AVAILABLE_CNF_T_AVAILABLE_NUM          = 0x101,
    /* the max string length of queried EF files, such as alpha string length of an EF_GAS entry. */
    /* type = uint32_t */
    MIPC_PHB_GET_PHB_AVAILABLE_CNF_T_MAX_LEN                = 0x102,
};

    /* MIPC_MSG.PHB_READY_STATE_IND */
enum mipc_phb_ready_state_ind_tlv_enum {
    mipc_phb_ready_state_ind_tlv_NONE = 0,
    /* phb ready indication, 1:ready  0:not ready */
    /* type = uint8_t */
    MIPC_PHB_READY_STATE_IND_T_READY                        = 0x100,
};




#endif /* __MIPC_MSG_PHB_CONST_H__ */
