import { PaperPage } from "~/components/ui/PaperPage";
import { CrachaPreview } from "./CrachaPreview";

export function CrachaPrint(){
  return(
    <div>
      <PaperPage orientation="horizontal" size="a4">
        <div class="size-full flex items-center justify-around">
          <CrachaPreview/>
          <CrachaPreview/>
          <CrachaPreview/>
          <CrachaPreview/>
        </div>
      </PaperPage>
    </div>
  );
}