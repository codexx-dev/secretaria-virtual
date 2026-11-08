import { AppLayout } from "~/components/layouts/AppLayout";
import { CrachaScreen } from "~/features/cracha/CrachaScreen";

export default  function CrachaPage(){
  return(
    <AppLayout>
      <CrachaScreen/>
    </AppLayout>
  );
}