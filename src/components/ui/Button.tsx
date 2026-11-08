import { type JSX, splitProps } from "solid-js";

import { twMerge } from "tailwind-merge";

type Variant = "primary" | "secondary" | "outline" | "ghost" | "danger";
type Size = "sm" | "default" | "lg" | "icon";

type ButtonProps = JSX.ButtonHTMLAttributes<HTMLButtonElement> & {
  variant?: Variant;
  size?: Size;
};

const variants: Record<Variant, string> = {
  primary:
    "bg-blue-600 text-white shadow-sm hover:bg-blue-500 active:bg-blue-700",
  secondary:
    "bg-gray-100 text-gray-900 hover:bg-gray-200 active:bg-gray-300",
  outline:
    "border border-slate-300/40 text-slate-100 hover:bg-slate-100/10 active:bg-slate-100/15",
  ghost:
    "text-slate-300 hover:bg-slate-100/10 hover:text-slate-100 active:bg-slate-100/15",
  danger:
    "bg-red-600 text-white shadow-sm hover:bg-red-500 active:bg-red-700",
};

const sizes: Record<Size, string> = {
  sm: "px-2 py-1",
  default: "px-4 py-2",
  lg: "h-12 px-6 text-base",
  icon: "h-8 w-8",
};

export default function Button(props: ButtonProps) {
  const [local, rest] = splitProps(props, ["class", "children"]);

  return (
    <button
      {...rest}
      class={twMerge(
        "relative inline-flex select-none items-center justify-center gap-2 overflow-hidden rounded-lg font-semibold",
        "transition duration-150 ease-out",
        "active:scale-[0.97]",
        "focus:outline-none focus-visible:ring-2 focus-visible:ring-blue-400/70",
        "focus-visible:ring-offset-2 focus-visible:ring-offset-slate-800",
        "disabled:cursor-not-allowed disabled:opacity-50 disabled:active:scale-100",
        sizes[props.size ?? "default"],
        variants[props.variant ?? "primary"],
        local.class,
      )}
    >
      {local.children}
    </button>
  );
}
