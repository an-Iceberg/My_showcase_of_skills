import Router from "express";
import { home } from "../controllers/homeController";
import { contact } from "../controllers/contactController";
import { about } from "../controllers/aboutController";

export const router = Router();

router.use(Router.urlencoded({ extended: true }));

router.get('/', home);
router.get('/contact', contact);
router.get('/about', about)
