import { AppLayout } from "~/components/layouts/AppLayout";
import { HomeScreen } from "~/features/home/HomeScreen";

export default function Home() {
  return (
    <AppLayout>
      <HomeScreen />
    </AppLayout>
  );
}
