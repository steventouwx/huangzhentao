package libtrm_build

import (
	"strings"
	"android/soong/android"
	"android/soong/cc"
	"fmt"
)

func init() {
	fmt.Println("libtrm_build init start")
	android.RegisterModuleType("libtrm_build", libtrm_buildDefaultsFactory)
}

func libtrm_buildDefaultsFactory() android.Module {
	module := cc.DefaultsFactory()
	android.AddLoadHook(module, libtrm_buildDefaults)
	return module
}

func libtrm_buildDefaults(ctx android.LoadHookContext) {
	type props struct {
        Srcs []string
        Include_dirs []string
        Cflags []string
        Shared_libs []string
    }
	p := &props{}
    vars := ctx.Config().VendorConfig("mtkPlugin")

	support_tmd := strings.ToLower(vars.String("MTK_THIN_MD_TELEPHONY_SUPPORT_MODE"))
	fmt.Println("libtrm_build MTK_THIN_MD_TELEPHONY_SUPPORT_MODE:", support_tmd)
	if support_tmd == "0" {
		fmt.Println("libtrm_build not support MTK_THIN_MD_TELEPHONY_SUPPORT_MODE")
	} else if support_tmd == "1" || support_tmd == "2" {
		p.Cflags = append(p.Cflags,"-DMTK_RIL_THIN_MD_SUPPORT")
		fmt.Println("libtrm_build support MTK_THIN_MD_TELEPHONY_SUPPORT_MODE")
	} else {
		fmt.Println("libtrm_build not support MTK_THIN_MD_TELEPHONY_SUPPORT_MODE")
	}
	ctx.AppendProperties(p)
}
