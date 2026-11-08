import { JSX } from "solid-js";

import { appStyle } from "~/app";
import InfoIcon from "~/assets/info.svg";

type AppLayoutProps = {
  children?: JSX.Element;
};

export function AppLayout(props: AppLayoutProps) {
  return (
    <main
      class={`h-screen flex flex-col ${appStyle.bgCol2} ${appStyle.textCol1}`}
    >
      <nav
        class={`no-print relative px-5 flex flex-row justify-center items-center gap-3 ${appStyle.bgCol1}`}
      >
        <a
          href="/"
          class="py-5 text-4xl font-bold text-center cursor-pointer hover:underline"
        >
          SECRETARIA VIRTUAL
        </a>

        <div class="absolute mx-5 bottom-0 right-0 text-lg hover:underline flex flex-row gap-5">
          <a class="flex gap-1" href="/sobre">
            {InfoIcon}
            Sobre...
          </a>
        </div>
      </nav>

      <div class="min-h-0 flex-1">{props.children}</div>
    </main>
  );
}
