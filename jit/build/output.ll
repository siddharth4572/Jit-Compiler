; ModuleID = 'jit'
source_filename = "jit"

@fmt = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(ptr, ...)

define i32 @main() {
entry:
  %result = alloca double, align 8
  %a = alloca double, align 8
  store double 1.000000e+01, ptr %a, align 8
  %a1 = load double, ptr %a, align 8
  %cmptmp = fcmp ult double %a1, 0.000000e+00
  %booltmp = uitofp i1 %cmptmp to double
  %ifcond = fcmp one double %booltmp, 0.000000e+00
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %entry
  store double 1.000000e+00, ptr %result, align 8
  br label %ifcont

else:                                             ; preds = %entry
  store double 0.000000e+00, ptr %result, align 8
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %result2 = load double, ptr %result, align 8
  %0 = call i32 (ptr, ...) @printf(ptr @fmt, double %result2)
  ret i32 0
}
