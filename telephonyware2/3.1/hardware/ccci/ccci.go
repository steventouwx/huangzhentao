package mtkCcciUtil

import (
	"android/soong/android"
	"android/soong/cc"
)

func mtkCcciUtilDefaults(ctx android.LoadHookContext) {
	type props struct {
		Cflags []string
	}
	p := &props{}
	vars := ctx.Config().VendorConfig("mtkPlugin")
	v1 := vars.String("MTK_MD_SBP_CUSTOM_VALUE")
	if v1 != "" {
		p.Cflags = append(p.Cflags, "-DMD_SBP_CUSTOM_VALUE="+v1)
	}
	v2 := vars.String("MTK_MD2_SBP_CUSTOM_VALUE")
	if v2 != "" {
		p.Cflags = append(p.Cflags, "-DMD2_SBP_CUSTOM_VALUE="+v2)
	}
	if vars.Bool("MTK_ECCCI_C2K") {
		p.Cflags = append(p.Cflags, "-DMTK_ECCCI_C2K")
	}
	ctx.AppendProperties(p)
}

func init() {
	android.RegisterModuleType("mtk_ccci_util_defaults", mtkCcciUtilDefaultsFactory)
}

func mtkCcciUtilDefaultsFactory() android.Module {
	module := cc.DefaultsFactory()
	android.AddLoadHook(module, mtkCcciUtilDefaults)
	return module
}

