import { splitProps, type JSX } from "solid-js";
import { twMerge } from "tailwind-merge";

type ButtonProps = JSX.ButtonHTMLAttributes<HTMLButtonElement> & {
  variant?: "primary" | "secondary" | "cancel";
};

const variants = {
  primary: "bg-blue-600 text-white hover:bg-blue-700",
  secondary: "bg-gray-100 text-gray-900 hover:bg-gray-200",
  cancel: "bg-red-600 text-white hover:bg-gray-700",
};

export default function Button(props: ButtonProps) {
  const [local, rest] = splitProps(props, ["variant", "class", "children"]);

  return (
    <button
      {...rest}
      class={twMerge(
        `inline-flex items-center justify-center rounded-lg
        px-4 py-2.5 text-sm font-semibold transition focus:outline-none
        focus:ring-2 focus:ring-blue-500/40 disabled:cursor-not-allowed
        disabled:opacity-60 ${variants[local.variant ?? "primary"]}`, local.class ?? "")}
    >
      {local.children}
    </button>
  );
}