package mtklibmipc

import (
    "strings"
    "android/soong/android"
    "android/soong/cc"
    "fmt"
)

func init() {
    fmt.Println("mtklibmipc init start")
    android.RegisterModuleType("mtklibmipc", mtklibmipcDefaultsFactory)
}

func mtklibmipcDefaultsFactory() android.Module {
    module := cc.DefaultsFactory()
    android.AddLoadHook(module, mtklibmipcDefaults)
    return module
}

func mtklibmipcDefaults(ctx android.LoadHookContext) {
    type props struct {
        Srcs []string
        Include_dirs []string
        Cflags []string
        Shared_libs []string
    }
    p := &props{}
    vars := ctx.Config().VendorConfig("mtkPlugin")

    support_tmd := strings.ToLower(vars.String("MTK_THIN_MD_TELEPHONY_SUPPORT_MODE"))
    fmt.Println("libmipc MTK_THIN_MD_TELEPHONY_SUPPORT_MODE:", support_tmd)
    if support_tmd == "0" {
        fmt.Println("libmipc not support MTK_THIN_MD_TELEPHONY_SUPPORT_MODE")
    } else {
        p.Cflags = append(p.Cflags,"-DMTK_RIL_THIN_MD_SUPPORT")
        fmt.Println("libmipc support MTK_THIN_MD_TELEPHONY_SUPPORT_MODE")
    }
}
